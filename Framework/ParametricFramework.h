#ifndef __PARAMETRIC_FRAMEWORK__
#define __PARAMETRIC_FRAMEWORK__



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

#include "../ShapeStructure/ShapeStructure.h"
#include "../Utility/Utility.h"
#include "../Utility/PredicateUtil.h"

#include <vector>

using namespace std;
using namespace llvm;

class ParametricFramework
{
public:
	using FlowSet = vector<ShapeStructure>;
	ParametricFramework(ShapeStructure&, Function*);
	virtual ~ParametricFramework() {}
	// predicate update specified
	// use control flow components
	virtual FlowSet flowFunction(FlowSet& currFlow,
		Instruction* inst,
		unsigned nfield) = 0;
	virtual FlowSet blockInitialization() = 0;
	virtual FlowSet entryInitialization() = 0;
	virtual FlowSet copy(FlowSet&) = 0;
	virtual FlowSet merge(FlowSet&, FlowSet&) = 0;


	void runAnalysis(Function*);

protected:

	void initializeStateMap(Function* F, map<int, FlowSet>& last, map<int, FlowSet>& curr);

	// helper function
	bool isSameFlowSet(FlowSet& src1, FlowSet& src2);

	void printFlowSet(FlowSet& src);

	FlowSet accumulateBranch(vector<FlowSet>&);
	
	FlowSet produceNextStates(FlowSet& currState, Instruction*);

	bool hasStateChange(FlowSet&, FlowSet&);
	// the input structure
	ShapeStructure input;
	unsigned nfield;
	Type* struct_type;
};

#endif
