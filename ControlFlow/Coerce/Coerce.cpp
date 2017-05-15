	

#include "Coerce.h"

using namespace llvm;
using namespace std;

bool Coerce::isValid(ShapeStructure& ss)
{
	set<string> allIndividuals{ss.getIndividualsRange()};

	for (LogicPredicate* lp : allConstrains) {
		for (string indiv : allIndividuals) {
			vector<string> vs{indiv};
			PredicateArg args{vs};
			int val = lp->apply(ss, args);
			if (val != 2) { return false; }
		}
	}
	return true;
}

Coerce::FlowSet Coerce::flowFunction(FlowSet& currFlow,
	Instruction* inst,
	unsigned nfield, 
	vector<int>& predicateIndex,
	map<int, LogicPredicate*>& allPreconditions,
	map<string, LogicPredicate*>& updatePredicates)
{
	vector<ShapeStructure> res{ currFlow };
	
	for (unsigned i = 0; i < currFlow.size(); ++i) {
		ShapeStructure ss = currFlow[i];
		if (isValid(ss)) {
			res.push_back(ss);
		}
	}

	return move(res);
}