

#include "Update.h"
#include "../../Utility/Utility.h"
#include "../../Utility/PredicateUtil.h"

using namespace std;
using namespace llvm;


// the transformation on a given shape structure
// at a given instruction
ShapeStructure Update::instFlow(ShapeStructure& from, Instruction* ins, 
	unsigned nfield, map<string, LogicPredicate*>& updatePredicates)
{
	ShapeStructure ss{ from };

	if (CallInst* ci = dyn_cast<CallInst>(ins)) {
		// only cares about malloc and free
		// x1 = malloc()
		// x1 = free()
		Function* called_func = ci->getCalledFunction();
		string called_func_name{ called_func->getName().str() };

		Instruction* user = dyn_cast<Instruction>(*(ci->user_begin()));

		string x1 = user->getName().str();
		if (called_func_name == "malloc") {
			// malloc
			ss.createIndividual();
			// only unary predicate
			for (auto& kvp : updatePredicates) {
				string pred{kvp.first};
				LogicPredicate* lp = kvp.second;
				// update all individuals
				set<string> allIndividuals{ss.getIndividualsRange()};
				// iterate
				for (string v : allIndividuals) {
					vector<string> vs{v};
					PredicateArg args{vs};
					int val = lp->apply(ss, args);
					ss.setUnaryPredicate(x1, v, val);
				}

			}
		}
	}
	else if (LoadInst* li = dyn_cast<LoadInst>(ins)) {
		// x1 = x2->n
		// usually couple with a getelementptr:
		// %tmp13 = getelementptr %struct.node, %struct.node* %.0, i32 0, i32 0
		// %tmp14 = load %struct.node*, %struct.node** %tmp13
		Value* load_from = li->getOperand(0);

		if (doInvokeMemory(load_from, nfield)) {
			Value* from_unary = traceIndividualBack(load_from);
			string x2 = from_unary->getName().str();
			string x1 = li->getName().str();

			for (auto& kvp : updatePredicates) {
				string pred{kvp.first};
				LogicPredicate* lp = kvp.second;
				// all individuals
				set<string> allIndividuals{ss.getIndividualsRange()};
				// only unary predicates are possible
				for (string v : allIndividuals) {
					vector<string> vs{v};
					PredicateArg args{vs};
					int val = lp->apply(ss, args);
					//outs() << "update::load::" << x1 << " = " << val << "\n";
					ss.setUnaryPredicate(x1, v, val);
				}
			}
		}

	}
	else if (StoreInst* si = dyn_cast<StoreInst>(ins)) {
		// x1->n = x2;
		// usually couple with a getelemenptr:
		// %tmp16 = getelementptr %struct.node, %struct.node* %tmp12, i32 0, i32 0
		// store %struct.node* %arg, %struct.node** %tmp16
		Value* store_from = si->getOperand(0);
		Value* store_to = si->getOperand(1);

		if (doInvokeMemory(store_to, nfield)) {
			Value* from_unary = traceIndividualBack(store_from);
			Value* to_unary = traceIndividualBack(store_to);

			string x2 = from_unary->getName().str();
			string x1 = to_unary->getName().str();

			for (auto& kvp : updatePredicates) {
				string pred{kvp.first};
				LogicPredicate* lp = kvp.second;

				set<string> allIndividuals{ss.getIndividualsRange()};
				// only binary predicates are possible
				for (string v1 : allIndividuals) {
					for (string v2 : allIndividuals) {
						vector<string> vs{v1, v2};
						PredicateArg args{vs};
						int val = lp->apply(ss, args);
						ss.setBinaryPredicate(pred, v1, v2, val);
					}
				}
			}
		}

	}
	// by default
	// we do nothing about the flow
	return move(ss);
}

// flow function provided
Update::FlowSet Update::flowFunction(FlowSet& currFlow,
	Instruction* inst,
	unsigned nfield,
	vector<int>& predicateIndex,
	map<int, LogicPredicate*>& allPreconditions, 
	map<string, LogicPredicate*>& updatePredicates)
{
	vector<ShapeStructure> res{};

	// iterate through the flow set
	for (ShapeStructure& ss : currFlow) {
		// update basd on the preconditions
		if (doesSatisfyPrecondition(ss, predicateIndex, allPreconditions)) {
			res.push_back(instFlow(ss, inst, nfield, updatePredicates));
		}
		else {
			res.push_back(ss);
		}
	}

	return move(res);
}