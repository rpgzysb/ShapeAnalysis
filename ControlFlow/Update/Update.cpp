

#include "Update.h"
#include "../../Utility/Utility.h"
#include "../../Utility/PredicateUtil.h"

using namespace std;
using namespace llvm;


ShapeStructure Update::instFlow(ShapeStructure& from, Instruction* ins, unsigned nfield)
{
	ShapeStructure ss{ from };

	if (CallInst* ci = dyn_cast<CallInst>(ins)) {
		// only cares about malloc and free
		Function* called_func = ci->getCalledFunction();
		string called_func_name{ called_func->getName().str() };
		if (called_func_name == "malloc") {
			// malloc
		}
		else if (called_func_name == "free") {
			// free
		}
	}
	else if (LoadInst* li = dyn_cast<LoadInst>(ins)) {
		// x1 = x2->n
		// usually couple with a getelementptr:
		// %tmp13 = getelementptr %struct.node, %struct.node* %.0, i32 0, i32 0
		// %tmp14 = load %struct.node*, %struct.node** %tmp13
		Value* load_from = li->getOperand(0);

		if (doInvokeMemory(load_from, nfield)) {
			outs() << "load inside\n";
			Value* from_unary = traceIndividualBack(load_from);
			string x2 = from_unary->getName().str();
			string x1 = li->getName().str();

			outs() << "x1 = " << x1 << ", x2 = " << x2 << "\n";



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
			outs() << "store inside\n";
			Value* from_unary = traceIndividualBack(store_from);
			Value* to_unary = traceIndividualBack(store_to);

			string x2 = from_unary->getName().str();
			string x1 = to_unary->getName().str();

			outs() << "x1 = " << x1 << ", x2 = " << x2 << "\n";

	

		}

	}
	// by default
	// we do nothing about the flow
	return move(ss);
}

Update::FlowSet Update::flowFunction(FlowSet& currFlow,
	Instruction* inst,
	unsigned nfield,
	vector<int>& predicateIndex,
	map<int, LogicPredicate*>& allPreconditions)
{
	vector<ShapeStructure> res{};

	for (ShapeStructure& ss : currFlow) {
		if (doesSatisfyPrecondition(ss, predicateIndex, allPreconditions)) {
			res.push_back(instFlow(ss, inst, nfield));
		}
		else {
			res.push_back(ss);
		}
	}

	return move(res);
}