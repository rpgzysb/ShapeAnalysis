

#ifndef __PREDICATE_UTILITY__
#define __PREDICATE_UTILITY__

/*
 * Utility for predicates.
*/

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

// make unary predicate from name
LogicPredicate* makeUnaryPredicate(string upred);

// make binary predicate from name
LogicPredicate* makeBinaryPredicate(string bpred);

// make all predicates from name
map<string, LogicPredicate*> makeAllPredicate(set<string>& upreds, set<string>& bpreds);

// make preconditions from branch instruction
LogicPredicate* makePreconditions(Instruction* inst, map<string, LogicPredicate*>& allPredicates);

// collect all preconditions ast in a function
map<int, LogicPredicate*> collectAllPreconditions(Function* F, map<string, LogicPredicate*>& allPredicates);

// trace the getelementptr variable back
Value* traceIndividualBack(Value* inst);

// make update predicates in an instruction
map<string, LogicPredicate*> makeUpdatePredicate(Instruction* inst,
	map<string, LogicPredicate*>& allPredicates, unsigned nfield);

// see if the instruction satisfies the precondtion
bool doesSatisfyPrecondition(ShapeStructure& ss, vector<int>& predicateIndex,
	map<int, LogicPredicate*>& allPreconditions);

// collect all update predicates
map<int, map<string, LogicPredicate*>> collectAllUpdatePredicate(Function* F,
	map<string, LogicPredicate*>& allPredicates, unsigned nfield);

// helper function for keyset in java
set<string> convertMapSet(map<string, int>& mp);

// initialize the predicates for each instruction
map<int, vector<int>> initializeSatisfyPredicate(Function* F);

unordered_set<LogicPredicate*> collectAllConstratins(set<string>& upreds, 
	set<string>& bpreds, map<string, LogicPredicate*>& allLogicPredicates);

#endif 