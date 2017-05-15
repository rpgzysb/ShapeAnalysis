#ifndef __PREDICATE__
#define __PREDICATE__

#include "PredicateArg.h"
#include "../ShapeStructure/ShapeStructure.h"

using namespace std;


// the predicate base class
// interface of apply and getName
class LogicPredicate
{
public:
	LogicPredicate(string npred_): npred{npred_} {}
	virtual int apply(ShapeStructure& ss, PredicateArg& args) = 0;
	string getName() { return npred; }
protected:
	string npred;
};


#endif