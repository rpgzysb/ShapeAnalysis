

#include "Focus.h"
#include "../../Preidcate/AndPredicate.h"
#include "../../Preidcate/TruePredicate.h"

using namespace std;
using namespace llvm;


vector<LogicPredicate*> splitParts(LogicPredicate* formula)
{
	vector<LogicPredicate*> parts{};

	if (AndPredicate* ap = dynamic_cast<AndPredicate>(formula)) {
		// and formula 
		// should be split
		parts.push_back(ap->getPred(0));

		vector<LogicPredicate*> remains{ splitParts(ap->getPred(1)) };
		for (LogicPredicate* lp : remains) {
			parts.push_back(lp);
		}
	}
	else {
		parts.push_back(formula);
	}


	return move(parts);
}

vector<LogicPredicate*> splitFormula(vector<LogicPredicate*> parts)
{
	vector<LogicPredicate*> phis{};

	phis.push_back()

	return move(phis);
}


Focus::FlowSet Focus::focusOnConjunction(FlowSet& xs, 
	LogicPredicate* formula)
{
	vector<LogicPredicate*> parts = splitParts(formula);
	vector<LogicPredicate*> phis = splitFormula(parts);
	
	vector<ShapeStructure> res{};
	for (unsigned i = 0; i < phis.size(); ++i) {
		res = focusOnLiteral(res, phis[i], parts[i]);
	}

	return move(res);
}


Focus::FlowSet Focus::makeFocus(ShapeStructure& ss, 
		LogicPredicate* formula)
{
	vector<ShapeStructure> xs{ss};
	vector<ShapeStructure> res{focusOnConjunction(xs, formula)};

	return move(res);
}


Focus::FlowSet Focus::flowFunction(FlowSet& currFlow,
		Instruction* inst,
		unsigned nfield, 
		vector<int>& predicateIndex,
		map<int, LogicPredicate*>& allPreconditions,
		map<string, LogicPredicate*>& updatePredicates)
{
	return currFlow;
}