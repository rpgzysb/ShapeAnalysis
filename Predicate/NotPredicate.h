#ifndef __NOT_PREDICATE__
#define __NOT_PREDICATE__

#include <algorithm>

#include "LogicPredicate.h"

using namespace std;

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