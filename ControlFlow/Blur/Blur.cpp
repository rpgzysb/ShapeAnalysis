
#include <unordered_set>
#include "Blur.h"

using namespace std;
using namespace llvm;

// normal constructor
// collect all unary predicate information
Blur::Blur(map<string, LogicPredicate*>& allLogicPredicates, 
		set<string>& unaryPredicates)
{
	for (string upred : unaryPredicates) {
		LogicPredicate* lp = allLogicPredicates[upred];
		allUnaryPredicates.insert(lp);
	}

	binaryPredicates.insert(allLogicPredicates["_n"]);
	summaryPredicate = allLogicPredicates["_sm"];

}

// see if two structures are similar
// so that they could be merged
ShapeStructure similarStructure(ShapeStructure& ss)
{
	
	map<string, int> individuals{ ss.getIndividuals() };
	map<int, map<int, int>> upreds{ ss.getUnaryPredicates() };

	unsigned category = 0;
	vector<int> kindsIndividuals{};
	map<int, vector<string>> classifications{};

	for (auto indivp : individuals) {
		string v = indivp.first;
		int v_val = indivp.second;
		// comparison and put into correct classification
		if (category == 0) {
			// initial
			classifications[category].push_back(v);
			kindsIndividuals.push_back(v_val);
			category++;
		}
		else {
			// compare against all kinds of individuals so far
			unsigned i = 0;
			for (; i < kindsIndividuals.size(); ++i) {
				int to_compare = kindsIndividuals[i];
				map<int, int> to_compare_state{ upreds[to_compare] };
				map<int, int> this_state{ upreds[v_val] };
				if (to_compare_state == this_state) {
					classifications[i].push_back(v);
					break;
				}
			}
			// at the end
			// that means the individual form a category itself
			if (i == kindsIndividuals.size()) {
				classifications[category].push_back(v);
				kindsIndividuals.push_back(v_val);
				category++;
			}
		}
	}

	// no summary node
	if (category == individuals.size()) {
		return ss;
	}
	else {
		// create new individuals
		map<string, int> new_indivs{};
		// create new unary predicate map
		map<int, map<int, int>> new_unary_predicates;
		// create new binary predicate map
		map<int, 
				map<int, map<int, int>>
				> new_binary_predicates;
		for (auto kvp : classifications) {
			vector<string> this_category{ kvp.second };
			if (this_category.size() != 0) {
				string summary_v = this_category[0];
				int summary_v_val = individuals[summary_v];
				new_indivs[summary_v] = summary_v_val;
				new_unary_predicates[summary_v_val] = upreds[summary_v_val];

				
				
			}
		}


	}



}


Blur::FlowSet Blur::flowFunction(FlowSet& currFlow,
		Instruction* inst,
		unsigned nfield, 
		vector<int>& predicateIndex,
		map<int, LogicPredicate*>& allPreconditions,
		map<string, LogicPredicate*>& updatePredicates)
{

}