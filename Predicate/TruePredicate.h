#ifndef __TRUE_PREDICATE__
#define __TRUE_PREDICATE__

#include <algorithm>

#include "LogicPredicate.h"

using namespace std;

// always evaluate to true
class TruePredicate : public LogicPredicate
{
public:
	TruePredicate(string npred_) :	LogicPredicate{ npred_ } {}
	virtual int apply(ShapeStructure& ss, PredicateArg& args) override
	{
		return 2;
	}
	LogicPredicate* getPred(int n)
	{
		if (n == 0) { return phi1; }
		else { return phi2; }
	}
};

#endif