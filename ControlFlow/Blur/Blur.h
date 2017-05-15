#ifndef __BLUR__
#define __BLUR__

#include "../ControlFlow.h"

using namespace std;
using namespace llvm;


class Blur : public ControlFlow
{
public:
	Blur(map<string, LogicPredicate*>& allLogicPredicates_, 
		set<string>& unaryPredicates);
	~Blur() {}

	using FlowSet = vector<ShapeStructure>;
	virtual FlowSet flowFunction(FlowSet& currFlow,
		Instruction* inst,
		unsigned nfield, 
		vector<int>& predicateIndex,
		map<int, LogicPredicate*>& allPreconditions,
		map<string, LogicPredicate*>& updatePredicates) override;
private:
	unordered_set<LogicPredicate*> allUnaryPredicates;
	unordered_set<LogicPredicate*> binaryPredicates;
	LogicPredicate* summaryPredicate;
};


#endif