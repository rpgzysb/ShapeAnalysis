
#include "PredicateUtil.h"
#include "llvm/IR/InstrTypes.h"
#include "../ControlFlow/Blur/Blur.h"
#include "../ControlFlow/Coerce/Coerce.h"
#include "../ControlFlow/Focus/Focus.h"
#include "../ControlFlow/Update/Update.h"
#include "../Predicate/UnaryPredicate.h"
#include "../Predicate/BinaryPredicate.h"
#include "../Predicate/AndPredicate.h"
#include "../Predicate/EqualPredicate.h"
#include "../Predicate/ExistPredicate.h"
#include "../Predicate/ForallPredicate.h"
#include "../Predicate/ImplyPredicate.h"
#include "../Predicate/NotPredicate.h"
#include "../Predicate/OrPredicate.h"

using namespace llvm;
using namespace std;


LogicPredicate* makeUnaryPredicate(string upred)
{
	return new UnaryPredicate(upred);
}

LogicPredicate* makeBinaryPredicate(string bpred)
{
	return new BinaryPredicate(bpred);
}
 
map<string, LogicPredicate*> makeAllPredicate(set<string>& upreds, set<string>& bpreds)
{
	map<string, LogicPredicate*> allPreds{};

	for (string upred : upreds) {
		allPreds[upred] = makeUnaryPredicate(upred);
	}

	for (string bpred : bpreds) {
		allPreds[bpred] = makeBinaryPredicate(bpred);
	}

	return move(allPreds);
}

LogicPredicate* makePreconditions(Instruction* inst, map<string, LogicPredicate*>& allPredicates)
{
	if (BranchInst* bi = dyn_cast<BranchInst>(inst)) {
		if (bi->isConditional()) {
			Value* cmp_inst = bi->getCondition();
			if (CmpInst* ci = dyn_cast<CmpInst>(cmp_inst)) {
				outs() << "makePreconditions::";
				ci->print(outs());
				outs() << "\n";

				vector<Value*> oprs{};
				for (auto cop = ci->op_begin(); cop != ci->op_end(); ++cop) {
					Value* val = *cop;
					oprs.push_back(val);
				}

				outs() << "makePreconditions::ops\n";
				for (Value* cinst : oprs) {
					cinst->print(outs());
					outs() << "\n";
				}

				Value* x1 = oprs[0];
				Value* x2 = oprs[1];

				string x1_name{ x1->getName().str() };
				string x2_name{ x2->getName().str() };

				bool is_null = isa<ConstantPointerNull>(x2);
				LogicPredicate* lp = nullptr;
				if (is_null) {
					lp = new ExistPredicate("exist " + x1_name, allPredicates[x1_name], 0);
				}
				else {
					lp = new ForallPredicate("forall " + x1_name + " == " + x2_name,
						new ImplyPredicate(x1_name + " <==> " + x2_name, 
							allPredicates[x1_name], allPredicates[x2_name]), 
						0);
				}
					

				CmpInst::Predicate pred = ci->getPredicate();
				if (pred == CmpInst::ICMP_NE) {
					// icmp ne
					return new NotPredicate("!" + lp->getName(), lp);
				}
				else if (pred == CmpInst::ICMP_EQ) {
					// icmp eq
					return lp;
				}
			}
		}
	}
	return nullptr;
}

map<int, LogicPredicate*> collectAllPreconditions(Function* F, map<string, LogicPredicate*>& allPredicates)
{
	hash<Instruction*> instHash{};

	map<int, LogicPredicate*> allPreconditions{};
	for (auto B = F->begin(); B != F->end(); ++B) {
		for (auto I = B->begin(); I != B->end(); ++I) {
			Instruction* inst = &*I;
			int inst_val = instHash(inst);
			allPreconditions[inst_val] = makePreconditions(inst, allPredicates);
		}
	}

	return move(allPreconditions);
}

Value* traceIndividualBack(Value* inst)
{
	// trace back from the load or store instruction
	// get the corresponding unary predicate
	if (CastInst* ci = dyn_cast<CastInst>(inst)) {
		// usually cast into the structure pointer
		// from malloc
		// %tmp12 = bitcast i8* %tmp11 to %struct.node*
		return inst;
	}
	else if (GetElementPtrInst* gepi = dyn_cast<GetElementPtrInst>(inst)) {
		// get a double pointer
		// record in the map
		// %tmp16 = getelementptr %struct.node, %struct.node* %tmp12, i32 0, i32 0
		// need to trace once more
		Value* src = gepi->getOperand(0);
		if (Instruction* oprnd = dyn_cast<Instruction>(src)) {
			return traceIndividualBack(oprnd);
		}
		else if (Argument* oprnd = dyn_cast<Argument>(src)) {
			return traceIndividualBack(oprnd);
		}
	}
	// by default
	// we do nothing about the flow
	return inst;
}

map<string, LogicPredicate*> makeUpdatePredicate(Instruction* inst, 
		map<string, LogicPredicate*>& allPredicates, unsigned nfield)
{
	map<string, LogicPredicate*> res{};

	if (CallInst* ci = dyn_cast<CallInst>(inst)) {
		// only cares about malloc and free
		// x1 = malloc()
		string x1 = ci->getName().str();
		Function* called_func = ci->getCalledFunction();
		string called_func_name{ called_func->getName().str() };
		if (called_func_name == "malloc") {
			// malloc
		}
		// x1 = free()
		else if (called_func_name == "free") {
			// free
		}
	}
	else if (LoadInst* li = dyn_cast<LoadInst>(inst)) {
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

			res[x1] = new ExistPredicate("exist " + x2 + "^n",
				new AndPredicate(x2 + "^n", allPredicates[x2], allPredicates["_n"]), 2);
		}
	}
	else if (StoreInst* si = dyn_cast<StoreInst>(inst)) {
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

			if (isa<ConstantPointerNull>(from_unary)) {
				// x1->n = null
				res["_n"] = new AndPredicate("_n^" + x1,
					allPredicates["_n"],
					new NotPredicate("!" + x1, allPredicates[x1]));
			}
			else {
				// x1->n = x2
				LogicPredicate* x1_and_x2 =
					new AndPredicate(x1 + "^" + x2,
						allPredicates[x1], allPredicates[x2]);
				res["_n"] = new OrPredicate("_n|" + x1_and_x2->getName(),
					allPredicates["_n"], x1_and_x2);
			}
		}
	}

	return move(res);
}

bool doesSatisfyPrecondition(ShapeStructure& ss, vector<int>& predicateIndex,
	map<int, LogicPredicate*>& allPreconditions)
{
	for (int idx : predicateIndex) {
		LogicPredicate* precond = allPreconditions[idx];
		PredicateArg dummy{};
		int val = precond->apply(ss, dummy);
		if (val != 1) { return false; }
	}

	return true;
}

map<int, map<string, LogicPredicate*>> collectAllUpdatePredicate(Function* F,
	map<string, LogicPredicate*>& allPredicates, unsigned nfield)
{
	hash<Instruction*> instHash{};

	map<int, map<string, LogicPredicate*>> allUpdatePredicates{};
	for (auto B = F->begin(); B != F->end(); ++B) {
		for (auto I = B->begin(); I != B->end(); ++I) {
			Instruction* inst = &*I;
			int inst_val = instHash(inst);
			allUpdatePredicates[inst_val] =
				makeUpdatePredicate(inst, allPredicates, nfield);
		}
	}

	return move(allUpdatePredicates);
}

set<string> convertMapSet(map<string, int>& mp)
{
	set<string> res{};
	for (auto kvp : mp) {
		res.insert(kvp.first);
	}
	return move(res);
}


map<int, vector<int>> initializeSatisfyPredicate(Function* F)
{
	hash<Instruction*> instHash{};

	map<int, vector<int>> satisfyPredicates{};
	for (auto B = F->begin(); B != F->end(); ++B) {
		for (auto I = B->begin(); I != B->end(); ++I) {
			Instruction* inst = &*I;
			int inst_val = instHash(inst);
			if (BranchInst* bi = dyn_cast<BranchInst>(inst)) {
				// branch dispatch
				if (bi->isConditional()) {
					unsigned num_successors{ bi->getNumSuccessors() };
					for (unsigned i = 0; i < num_successors; ++i) {
						BasicBlock* next_bb = bi->getSuccessor(i);
						Instruction* next_inst = &*next_bb->begin();
						int next_inst_val = instHash(next_inst);
						satisfyPredicates[next_inst_val].push_back(inst_val);
					}
				}
			}
		}
	}
	return move(satisfyPredicates);
}