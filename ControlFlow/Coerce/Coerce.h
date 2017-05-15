#ifndef __COERCE__
#define __COERCE__

#include <unordered_set>

#include "../ControlFlow.h"

using namespace std;
using namespace llvm;


class Coerce : public ControlFlow
{
public:
	Coerce(unordered_set<LogicPredicate*> allConstrains_) : 
		allConstrains{allConstrains_} 
	{}
	~Coerce() {}

	using FlowSet = vector<ShapeStructure>;
	virtual FlowSet flowFunction(FlowSet& currFlow,
		Instruction* inst,
		unsigned nfield, 
		vector<int>& predicateIndex,
		map<int, LogicPredicate*>& allPreconditions,
		map<string, LogicPredicate*>& updatePredicates) override;
private:
	unordered_set<LogicPredicate*> allConstrains;
	bool isValid(ShapeStructure& ss);
};


#endif