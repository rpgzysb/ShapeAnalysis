#ifndef __UNARY_PREDICATE__
#define __UNARY_PREDICATE__

#include "LogicPredicate.h"

using namespace std;

// the unary predicate
// looks like 
// x1(v) <---> x1 = npred, v = args
class UnaryPredicate : public LogicPredicate
{
public:
	UnaryPredicate(string npred_) : LogicPredicate{ npred_ } {}
	virtual int apply(ShapeStructure& ss, PredicateArg& args) override
	{
		return ss.getUnaryLogic(npred, args.getArg(0));
	}
};


#endif