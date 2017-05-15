

#ifndef __PREDICATE_UTILITY__
#define __PREDICATE_UTILITY__


#include <iostream>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <functional>

#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Support/Casting.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/IR/CFG.h"

#include "../ShapeStructure/ShapeStructure.h"
#include "../Predicate/LogicPredicate.h"

using namespace llvm;
using namespace std;

LogicPredicate* makeUnaryPredicate(string upred);

LogicPredicate* makeBinaryPredicate(string bpred);

map<string, LogicPredicate*> makeAllPredicate(set<string>& upreds, set<string>& bpreds);

LogicPredicate* makePreconditions(Instruction* inst, map<string, LogicPredicate*>& allPredicates);

map<int, LogicPredicate*> collectAllPreconditions(Function* F, map<string, LogicPredicate*>& allPredicates);

Value* traceIndividualBack(Value* inst);

map<string, LogicPredicate*> makeUpdatePredicate(Instruction* inst,
	map<string, LogicPredicate*>& allPredicates, unsigned nfield);

bool doesSatisfyPrecondition(ShapeStructure& ss, vector<int>& predicateIndex,
	map<int, LogicPredicate*>& allPreconditions);

map<int, map<string, LogicPredicate*>> collectAllUpdatePredicate(Function* F,
	map<string, LogicPredicate*>& allPredicates, unsigned nfield);


set<string> convertMapSet(map<string, int>& mp);

map<int, vector<int>> initializeSatisfyPredicate(Function* F);


#endif 