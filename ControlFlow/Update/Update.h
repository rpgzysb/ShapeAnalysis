#ifndef __UPDATE__
#define __UPDATE__

#include "../ControlFlow.h"

using namespace std;
using namespace llvm;


class Update : public ControlFlow
{
public:
	using FlowSet = vector<ShapeStructure>;
	virtual FlowSet flowFunction(FlowSet& currFlow,
		Instruction* inst,
		unsigned nfield, 
		vector<int>& predicateIndex,
		map<int, LogicPredicate*>& allPreconditions) override;

	ShapeStructure instFlow(ShapeStructure& from, 
		Instruction* ins, unsigned nfield);
};

#endif