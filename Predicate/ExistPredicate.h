#ifndef __EXIST_PREDICATE__
#define __EXIST_PREDICATE__

#include <algorithm>

#include "LogicPredicate.h"

using namespace std;

class ExistPredicate : public LogicPredicate
{
public:
	ExistPredicate(string npred_, LogicPredicate* phi_, int pos_) :
		LogicPredicate{ npred_ }, phi{ phi_ }, pos{ pos_ } {}
	virtual int apply(ShapeStructure& ss, PredicateArg& args) override
	{
		int max_val = numeric_limits<int>::min();
		map<string, int> all_individuals{ ss.getIndividuals() };
		
		for (auto kvp : all_individuals) {
			vector<string> pargs{};
			if (pos == 0) {
				// unary predicate
				pargs.push_back(kvp.first);
			}
			if (pos == 1) {
				// individual vary as the first arg
				pargs.push_back(kvp.first); pargs.push_back(args.getArg(0));
			}
			else if (pos == 2) {
				// individual vary as the second arg
				pargs.push_back(args.getArg(0)); pargs.push_back(kvp.first);
			}
			PredicateArg sub_args{pargs};
			int val = phi->apply(ss, sub_args);
			if (max_val < val) { max_val = val; }
		}

		return max_val;
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