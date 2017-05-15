
#ifndef __UTILITY__
#define __UTILITY__

/*
 * Header file for utility.
 * Mainly for shape structures.
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
#include "llvm/IR/Function.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/IR/CFG.h"


using namespace llvm;
using namespace std;

// determine whether ptr_type is a pointer of raw_type
bool isPointerOf(Type* ptr_type, Type* raw_type);

// determine whether ptr_type is a double pointer of raw_type
bool isDoublePointerOf(Type* ptr_type, Type* raw_type);

// get the data structue type in LLVM
Type* getStructType(Function* f);

// get the field index of the next pointer in LLVM
unsigned getFieldIndex(Type* struct_type);

// collect the name of all unary predicates
set<string> collectUnaryPredicate(Function* F);

// collect the name of all predicates
set<string> collectAllPredicates(Function* F);

// convert the set of names into a map 
map<string, int> covertSetMap(Function* F, set<string>& st, int& counter);

// see if the current instruction val involves memory operations
bool doInvokeMemory(Value* val, unsigned nfield);

// helper function to print the map
void printMap(map<string, int>& mp);

// check if elt is a vector element in vec
template <typename T>
bool isVectorElmt(vector<T>& vec, T& elt)
{
	for (T& v : vec) {
		if (v == elt) {
			return true;
		}
	}
	return false;
}

// check to see if sub is a subvector of vec
template <typename T>
bool isSubVector(vector<T>& vec, vector<T>& sub)
{
	for (T& s : sub) {
		if (!isVectorElmt(vec, s)) { return false; }
	}
	return true;
}

// check to see if vec1 and vec2 are the same vector
template <typename T>
bool isSameVector(vector<T>& vec1, vector<T>& vec2)
{
	return isSubVector(vec1, vec2) && isSubVector(vec2, vec1);
}

// create a vector union of both vec1 and vec2
template <typename T>
vector<T> vectorUnion(vector<T>& vec1, vector<T>& vec2)
{
	vector<T> res{ vec1 };
	for (T& v : vec2) {
		if (!isVectorElmt(res, v)) {
			res.push_back(v);
		}
	}
	return move(res);
}


#endif