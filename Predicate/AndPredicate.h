#ifndef __AND_PREDICATE__
#define __AND_PREDICATE__

#include <algorithm>

#include "LogicPredicate.h"

using namespace std;

// the and predicate
// a composite predicate
// looks like 
// (x1 ^ x2)(v1, v2) <---> (x1 | v2) = npred, v1, v2 = args
// can also be unary
class AndPredicate : public LogicPredicate
{
public:
	AndPredicate(string npred_, LogicPredicate* phi1_, LogicPredicate* phi2_) :
		LogicPredicate{ npred_ }, phi1{ phi1_ }, phi2{ phi2_ } {}
	virtual int apply(ShapeStructure& ss, PredicateArg& args) override
	{
		int val1 = phi1->apply(ss, args);
		int val2 = phi2->apply(ss, args);
		return min(val1, val2);
	}

	LogicPredicate* getPred(int n)
	{
		if (n == 0) { return phi1; }
		else { return phi2; }
	}
private:
	LogicPredicate* phi1;
	LogicPredicate* phi2;
};

#endif