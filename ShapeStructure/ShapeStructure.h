
#ifndef __SHAPE_STRUCTURE__
#define __SHAPE_STRUCTURE__

#include <string>
#include <map>
#include <iostream>
#include <set>
#include <functional>
#include <algorithm>
#include <memory>

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


#include "../Utility/Utility.h"


using namespace std;
using namespace llvm;

class ShapeStructure
{
public:
	// normal constructor
	ShapeStructure(map<string, int>&, shared_ptr<map<string, int>>,
		map<int, map<int, int>> unaryPredicates_,
	map<int, map<int, map<int, int>>> binaryPredicates_);
	// copy constructor
	ShapeStructure(const ShapeStructure&);
	// deconstructor
	~ShapeStructure() {}

	// getter
	map<string, int> getIndividuals() const { return individuals; }
	map<string, int> getPredicates() const { return *predicates; }
	shared_ptr<map<string, int>> getPredicatePtr() const { return predicates; }

	map<int, map<int, int>> getUnaryPredicates() const { return unaryPredicates; }

	map<int,
		map<int, map<int, int>>
	> getBinaryPredicates() const { return binaryPredicates; }

	// hash the index
	int getIndividualIdx(string);
	int getPredicateIdx(string);

	// get the shape structure
	int getUnaryLogic(string, string);
	int getBinaryLogic(string, string, string);

	// update the shape structure
	void setUnaryPredicate(string, string, int);
	void setBinaryPredicate(string, string, string, int);

	// create new individuals on malloc
	void createIndividual();

	// see if two structures are isomorphic
	bool operator==(const ShapeStructure&);

	set<string> getIndividualsRange() const { return individualsRange; }

	// new individual counter
	int indiv_count;
private:
	// the set of all individuals of this concrete shape structure
	map<string, int> individuals;
	set<string> individualsRange;
	// the set of all logic predicates of this concrete shape structure
	shared_ptr<map<string, int>> predicates;

	// the unary predicates table
	map<int, 
		map<int, int>
	> unaryPredicates;

	// the binary predicates table
	map<int, 
		map<int, map<int, int>>
	> binaryPredicates;

};



#endif