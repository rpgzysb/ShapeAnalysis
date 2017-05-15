#ifndef __SATISFY_PREDICATE__
#define __SATISFY_PREDICATE__

#include <algorithm>

#include "LogicPredicate.h"

using namespace std;


// the imply predicate
// looks like 
// (satisfy) (x1, x2) <---> satisfy(x1, x2) = npred, v1, v2 = args
class SatisfyPredicate : public LogicPredicate
{
public:
	SatisfyPredicate(string npred_, LogicPredicate* phi1_, LogicPredicate* phi2_) :
		LogicPredicate{ npred_ }, phi1{ phi1_ }, phi2{ phi2_ } {}
	virtual int apply(ShapeStructure& ss, PredicateArg& args) override
	{
		int val1 = phi1->apply(ss, args);
		int val2 = phi2->apply(ss, args);
		if (val1 == 2) {
			return (val2 == 2) ? 2 : 0;
		}
		else {
			return 0;
		}
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