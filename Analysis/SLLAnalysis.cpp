
#include <algorithm>

#include "SLLAnalysis.h"

using namespace std;
using namespace llvm;

// normal constructor
SLLAnalysis::SLLAnalysis(ShapeStructure& ss, Function* f,
	vector<ControlFlow*>& components,
	map<int, map<string, LogicPredicate*>>& allUpdatePredicates_,
	map<int, LogicPredicate*>& allPreconditions_,
	map<int, vector<int>>& satisfyPredicates_) :
	ParametricFramework{ ss, f },
	components{ components },
	allUpdatePredicates{ allUpdatePredicates_ },
	allPreconditions{ allPreconditions_ },
	satisfyPredicates{ satisfyPredicates_ }
{}

SLLAnalysis::FlowSet SLLAnalysis::blockInitialization()
{
	FlowSet fs{};
	fs.push_back(this->input);
	return move(fs);
}

SLLAnalysis::FlowSet SLLAnalysis::entryInitialization()
{
	FlowSet fs{};
	fs.push_back(this->input);
	return move(fs);
}

SLLAnalysis::FlowSet SLLAnalysis::copy(FlowSet& src)
{
	FlowSet res{ src };
	return move(res);
}

SLLAnalysis::FlowSet SLLAnalysis::merge(FlowSet& src1, FlowSet& src2)
{
	FlowSet res{ vectorUnion<ShapeStructure>(src1, src2) };
	return move(res);
}


// use different components to achieve the flow function
// update
SLLAnalysis::FlowSet SLLAnalysis::flowFunction(FlowSet& currFlow,
	Instruction* inst,
	unsigned nfield)
{
	FlowSet res{ currFlow };
	
	hash<Instruction*> instHash{};
	int inst_val = instHash(inst);
	// current preconditions
	vector<int> predicateIndex = satisfyPredicates[inst_val];
	// current update formula
	map<string, LogicPredicate*> updatePredicates = allUpdatePredicates[inst_val];

	outs() << "at instruction:\n\t";
	inst->print(outs());
	outs() << "\n";

	// pass into the components in order
	for (ControlFlow* cf : components) {
		res = cf->flowFunction(res, inst, nfield, 
			predicateIndex, allPreconditions, updatePredicates);
	}

	return move(res);
}

