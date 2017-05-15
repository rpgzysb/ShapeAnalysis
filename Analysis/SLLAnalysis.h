#ifndef __SLL_ANALYSIS__
#define __SLL_ANALYSIS__


#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/IR/CFG.h"


#include "../Framework/ParametricFramework.h"
#include "../ControlFlow/ControlFlow.h"
#include "../Predicate/LogicPredicate.h"

using namespace std;
using namespace llvm;


// implementation of the analysis
// on the singly linked list
class SLLAnalysis : public ParametricFramework
{
public:
	using FlowSet = ParametricFramework::FlowSet;
	// constructor
	SLLAnalysis(ShapeStructure&, Function*, 
		vector<ControlFlow*>&, 
		map<int, map<string, LogicPredicate*>>& allUpdatePredicates_,
		map<int, LogicPredicate*>& allPreconditions_,
		map<int, vector<int>>& satisfyPredicates_);

	~SLLAnalysis() {}

	// implementation of functions of the interface
	virtual FlowSet flowFunction(FlowSet& currFlow,
		Instruction* inst,
		unsigned nfield) override;
	
	virtual FlowSet blockInitialization() override;
	virtual FlowSet entryInitialization() override;
	virtual FlowSet copy(FlowSet&) override;
	virtual FlowSet merge(FlowSet&, FlowSet&) override;
private:
	// components
	map<int, map<string, LogicPredicate*>> allUpdatePredicates;
	map<int, LogicPredicate*> allPreconditions;
	map<int, vector<int>> satisfyPredicates;
	vector<ControlFlow*> components;
};


#endif
