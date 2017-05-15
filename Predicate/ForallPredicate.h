#ifndef __FORALL_PREDICATE__
#define __FORALL_PREDICATE__

#include "LogicPredicate.h"

using namespace std;

// the forall predicate
// a composite predicate
// pos provides indication which arg varies
// looks like 
// (forall v1) x(v1, v2) <---> forall x = npred, v1, v2 = args
// can also be unary
class ForallPredicate : public LogicPredicate
{
public:
	ForallPredicate(string npred_, LogicPredicate* phi_, int pos_) :
		LogicPredicate{ npred_ }, phi{ phi_ }, pos{ pos_ } {}
	virtual int apply(ShapeStructure& ss, PredicateArg& args) override
	{
		int min_val = numeric_limits<int>::max();
		map<string, int> all_individuals{ ss.getIndividuals() };

		for (auto kvp : all_individuals) {
			vector<string> pargs{};
			if (pos == 0) {
				// unary predicate
				pargs.push_back(kvp.first);
			}
			// for binary predicate
			else if (pos == 1) {
				// individual vary as the first arg
				pargs.push_back(kvp.first); pargs.push_back(args.getArg(0));
			}
			else {
				// individual vary as the second arg
				pargs.push_back(args.getArg(0)); pargs.push_back(kvp.first);
			}
			PredicateArg sub_args{pargs};
			int val = phi->apply(ss, sub_args);
			if (min_val > val) { min_val = val; }
		}

		return min_val;
	}

	LogicPredicate* getPred()
	{
		return phi;
	}
private:
	LogicPredicate* phi;
	int pos;
};

#endif