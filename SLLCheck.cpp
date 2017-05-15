#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/IR/CFG.h"


#include "Analysis/SLLAnalysis.h"
#include "Predicate/LogicPredicate.h"
#include "ControlFlow/Blur/Blur.h"
#include "ControlFlow/Coerce/Coerce.h"
#include "ControlFlow/Focus/Focus.h"
#include "ControlFlow/Update/Update.h"

using namespace std;
using namespace llvm;

namespace {

	class SLLCheck : public FunctionPass {
	public:
		static char ID;
		SLLCheck() : FunctionPass(ID) {}
		~SLLCheck() {}

		void getAnalysisUsage(AnalysisUsage& AU) const override 
		{
			AU.setPreservesAll();
		}

		bool doInitialization(Module& M) override 
		{
			return false;
		}

		ShapeStructure initializeConcreteStruct(map<string, int>& individualMap,
			shared_ptr<map<string, int>> preds)
		{
			// given unary predicate values
			map<int, map<int, int>> unaryPredicates{};
			unaryPredicates[5][0] = 2;

			// given binary predicate values
			map<int,
				map<int, map<int, int>>
			> binaryPredicates{};
			// initialize the beginning stage
			binaryPredicates[8][0][1] = 2;
			binaryPredicates[8][1][2] = 2;
			binaryPredicates[8][2][3] = 2;
			binaryPredicates[8][3][4] = 2;

			// the initial shape structure
			// to be analyzed
			ShapeStructure ss{
				individualMap,
				preds,
				unaryPredicates,
				binaryPredicates
			};

			return move(ss);
		}

		map<string, int> initializeIndividuals(Function* fblock, int& cnt)
		{
			// user specified structure node
			set<string> indivs{
				"u1", "u2", "u3", "u4", "u5"
			};
			// make internal individual mapping
			map<string, int> individualMap{
				covertSetMap(fblock, indivs, cnt)
			};

			outs() << "test collecting individuals\n";
			printMap(individualMap);

			return move(individualMap);
		}

		map<string, int> initializePredicates(Function* fblock, int& cnt)
		{
			// collect all predicates from the code
			set<string> allPreds{
				collectAllPredicates(fblock)
			};
			// make the map internally
			map<string, int> predicates{
				covertSetMap(fblock, allPreds, cnt)
			};

			outs() << "test collecting predicates\n";
			printMap(predicates);

			return move(predicates);
		}

		vector<ControlFlow*> initializeComponents()
		{
			Focus* sll_focus = new Focus();
			Update* sll_update = new Update();
			Coerce* sll_coerce = new Coerce();
			Blur* sll_blur = new Blur();
			vector<ControlFlow*> components{
				sll_focus, sll_update,
				sll_coerce, sll_blur
			};

			return move(components);
		}

		bool runOnFunction(Function& F) override
		{
			Function* fblock = &F;
			int cnt = 0;

			map<string, int> individualMap{ initializeIndividuals(fblock, cnt) };
			map<string, int> predicates{ initializePredicates(fblock, cnt) };
			// make a pointer to share among all shape structures
			shared_ptr<map<string, int>> preds = 
				make_shared<map<string, int>>(predicates);

			set<string> unaryPredicates{ convertMapSet(predicates) };
			unaryPredicates.erase("_n");
			set<string> binaryPredicates{ "_n" };
			// ast for predicates
			map<string, LogicPredicate*> allLogicPredicates{
				makeAllPredicate(unaryPredicates, binaryPredicates)
			};
			map<int, LogicPredicate*> allPreconditions{
				collectAllPreconditions(fblock, allLogicPredicates)
			};
			// update predicates for each instruction
			Type* struct_type = getStructType(fblock);
			map<int, map<string, LogicPredicate*>> allUpdatePredicates{
				collectAllUpdatePredicate(fblock, allLogicPredicates, getFieldIndex(struct_type))
			};
			// precondition predicates for each instruction
			map<int, vector<int>> satisfyPredicates{
				initializeSatisfyPredicate(fblock)
			};

			ShapeStructure ss{ initializeConcreteStruct(individualMap, preds) };
			vector<ControlFlow*> components{ initializeComponents() };
			// run the analysis
			SLLAnalysis slla{
				ss, 
				fblock, 
				components, 
				allUpdatePredicates, 
				allPreconditions,
				satisfyPredicates
			};
			slla.runAnalysis(fblock);


			for (unsigned i = 0; i < components.size(); ++i) {
				free(components[i]);
			}


			return false;
		}
	};

}

char SLLCheck::ID = 0;
static RegisterPass<SLLCheck> X("sll-check", "17665", false, false);