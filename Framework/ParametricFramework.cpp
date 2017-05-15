

#include "ParametricFramework.h"

#include <iostream>

using namespace std;
using namespace llvm;

ParametricFramework::ParametricFramework(ShapeStructure& ss, Function* f) :
	input{ ss } 
{
	struct_type = getStructType(f);
	nfield = getFieldIndex(struct_type);

	outs() << "nfield = " << nfield << "\n";
	outs() << "struct_type = ";
	struct_type->print(outs());
	outs() << "\n";
}

void ParametricFramework::printFlowSet(FlowSet& src1)
{
	outs() << "flow contains " << src1.size() << " shapes\n";
	for (int i = 0; i < src1.size(); ++i) {
		ShapeStructure ss = src1[i];
		outs() << "individuals: { ";
		for (auto indivvp : ss.getIndividuals()) {
			outs() << indivvp.first << "(" << indivvp.second << ") ";
		}
		outs() << " }\n";

		outs() << "predicates: { ";
		for (auto predvp : ss.getPredicates()) {
			outs() << predvp.first << "(" << predvp.second << ") ";
		}
		outs() << " }\n";

		outs() << "unary tables:\n";
		for (auto idxvp : ss.getUnaryPredicates()) {
			outs() << idxvp.first << ": ";
			for (auto uvp : idxvp.second) {
				outs() << uvp.second << "(" << uvp.first << ") ";
			}
			outs() << "\n";
		}
		 outs() << "binary tables:\n";
		 map<string, int> predicateMap = ss.getPredicates();
		 int n_pos = predicateMap["_n"];
		 for (auto idxvp : ss.getBinaryPredicates()[n_pos]) {
			outs() << idxvp.first << ": ";
		 	for (auto uvp : idxvp.second) {
		 		outs() << uvp.second << "(" << uvp.first << ") ";
			}
		 	outs() << "\n";
		 }
	}
	outs() << "\n";
}

ParametricFramework::FlowSet ParametricFramework::accumulateBranch(vector<FlowSet>& srcs)
{
	FlowSet curr = srcs[0];
	for (FlowSet& src : srcs) {
		curr = merge(src, curr);
	}

	return move(curr);
}


ParametricFramework::FlowSet ParametricFramework::produceNextStates(FlowSet& currState, Instruction* ins)
{
	hash<Instruction*> instHash{};
	int inst_val = instHash(ins);
	// TODO: change flow function
	FlowSet nextStates{flowFunction(currState, ins, nfield)};

	return move(nextStates);
}

bool ParametricFramework::isSameFlowSet(FlowSet& src1, FlowSet& src2)
{
	return isSameVector<ShapeStructure>(src1, src2);
}

bool ParametricFramework::hasStateChange(FlowSet& last, FlowSet& curr)
{
	return !isSameFlowSet(last, curr);
}


void ParametricFramework::initializeStateMap(Function* F,
	map<int, FlowSet>& lastFlow, map<int, FlowSet>& currFlow)
{
	hash<Instruction*> instHash{};
	Instruction* entry_ins = &*F->begin()->begin();
	int entry_val = instHash(entry_ins);
	FlowSet default_flow{ blockInitialization() };
	FlowSet entry_flow{ entryInitialization() };
	for (auto B = F->begin(); B != F->end(); ++B) {
		BasicBlock* bb = &*B;
		for (auto I = bb->begin(); I != bb->end(); ++I) {
			Instruction* inst = &*I;
			int inst_val = instHash(inst);
			lastFlow[inst_val] = (inst_val == entry_val) ?
				default_flow : entry_flow;
			currFlow[inst_val] = (inst_val == entry_val) ?
				default_flow : entry_flow;
		}
	}
}

map<int, vector<int>> initializePrevMap(Function* F)
{
	hash<Instruction*> instHash{};

	map<int, vector<int>> prevMap{};
	for (auto B = F->begin(); B != F->end(); ++B) {
		BasicBlock* bb = &*B;
		for (auto I = B->begin(); I != B->end(); ++I) {
			Instruction* inst = &*I;
			int inst_val = instHash(inst);

			if (inst == bb->getTerminator()) {
				// last instruction in basic block
				for (auto NB = succ_begin(bb); NB != succ_end(bb); ++NB) {
					BasicBlock* next_bb = *NB;
					Instruction* next_inst = &*next_bb->begin();
					int next_inst_val = instHash(next_inst);
					prevMap[next_inst_val].push_back(inst_val);
				}
			}
			else {
				++I; Instruction* next_inst = &*I; --I;
				int next_inst_val = instHash(next_inst);
				prevMap[next_inst_val].push_back(inst_val);
			}
		}
	}

	return move(prevMap);
}



void ParametricFramework::runAnalysis(Function* F)
{
	hash<Instruction*> instHash{};
	
	Instruction* entry_ins = &*F->begin()->begin();
	int entry_val = instHash(entry_ins);

	map<int, FlowSet> lastFlow{}, currFlow{};
	// initialize all the slots in the map
	initializeStateMap(F, lastFlow, currFlow);

	// initialize to true
	// before execution, see if the preconditions 
	// are satisfied
	map<int, vector<int>> satisfyPredicate{initializeSatisfyPredicate(F)};

	map<int, vector<int>> allPrevInsts{initializePrevMap(F)};

	bool change = true;
	while (change) {
		change = false;
		lastFlow = currFlow;

		for (auto B = F->begin(); B != F->end(); ++B) {
			BasicBlock* bb = &*B;
			for (auto I = bb->begin(); I != bb->end(); ++I) {
				Instruction* ins = &*I;
				outs() << "iterate at instruction: ";
				ins->print(outs());
				outs() << "\n";
				int ins_val = instHash(ins);
				vector<int> prevInsts{ allPrevInsts[ins_val] };
				// get the current state
				// the states of the previous instructions
				vector<FlowSet> tmp{};
				if (ins_val == entry_val) {
					tmp.push_back(entryInitialization());
				}
				else {
					for (int prev_inst : prevInsts) {
						tmp.push_back(currFlow[prev_inst]);
					}
				}

				FlowSet currState = 
					accumulateBranch(tmp);

				outs() << "original state\n";
				printFlowSet(currState);

				// update predicate
				FlowSet nextStates{ produceNextStates(currState, ins) };
				
				outs() << "nextStates update\n";
				printFlowSet(nextStates);
				// pass to next
				currFlow[ins_val] = nextStates;
				// see if the state changes
				if (hasStateChange(lastFlow[ins_val], currFlow[ins_val])) {
					change = true;
				}
			}
		}
	}




}