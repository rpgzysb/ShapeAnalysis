#ifndef __COERCE__
#define __COERCE__

#include "../ControlFlow.h"

using namespace std;
using namespace llvm;


class Coerce : public ControlFlow
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