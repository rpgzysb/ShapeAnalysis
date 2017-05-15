#ifndef __NOT_PREDICATE__
#define __NOT_PREDICATE__

#include <algorithm>

#include "LogicPredicate.h"

using namespace std;

// the not predicate
// a composite predicate
// looks like 
// (!x1)(v1, v2) <---> (!x1) = npred, v1, v2 = args
// can also be unary
class NotPredicate : public LogicPredicate
{
public:
	NotPredicate(string npred_, LogicPredicate* phi_) :
		LogicPredicate{ npred_ }, phi{ phi_ } {}
	virtual int apply(ShapeStructure& ss, PredicateArg& args) override
	{
		int val= phi->apply(ss, args);
		return 2 - val;
	}

	LogicPredicate* getPred()
	{
		return phi;
	}
private:
	LogicPredicate* phi;
};

#endif