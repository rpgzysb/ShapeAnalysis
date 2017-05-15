#ifndef __FOCUS__
#define __FOCUS__

#include "../ControlFlow.h"

using namespace std;
using namespace llvm;


class Focus : public ControlFlow
{
public:
	using FlowSet = vector<ShapeStructure>;
	virtual FlowSet flowFunction(FlowSet& currFlow,
		Instruction* inst,
		unsigned nfield, 
		vector<int>& predicateIndex,
		map<int, LogicPredicate*>& allPreconditions) override
	{
		return currFlow;
	}
};


#endif