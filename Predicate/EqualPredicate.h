#ifndef __EQUAL_PREDICATE__
#define __EQUAL_PREDICATE__

#include "LogicPredicate.h"

using namespace std;

// special predicate for equal
// x1 == x2
// different semantics than in boolean logic
class EqualPredicate : public LogicPredicate
{
public:
	EqualPredicate(string npred_) : LogicPredicate{ npred_ } {}
	virtual int apply(ShapeStructure& ss, PredicateArg& args) override
	{
		string v1{ args.getArg(0) };
		string v2{ args.getArg(1) };

		if (v1 != v2) {
			return 0;
		}
		else if ((v1 == v2) && (ss.getUnaryLogic("_sm", v1) == 0)) {
			return 2;
		}
		else {
			return 1;
		}
	}
};



#endif