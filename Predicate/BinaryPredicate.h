#ifndef __BINARY_PREDICATE__
#define __BINARY_PREDICATE__


#include "LogicPredicate.h"

using namespace std;

// the binary predicate
// looks like 
// n(v1, v2) <---> n = npred, v1, v2 = args
class BinaryPredicate : public LogicPredicate
{
public:
	BinaryPredicate(string npred_) : LogicPredicate{ npred_ } {}
	virtual int apply(ShapeStructure& ss, PredicateArg& args) override
	{
		return ss.getBinaryLogic(npred, args.getArg(0), args.getArg(1));
	}
};



#endif