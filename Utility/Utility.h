
#ifndef __UTILITY__
#define __UTILITY__


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

bool isPointerOf(Type* ptr_type, Type* raw_type);

bool isDoublePointerOf(Type* ptr_type, Type* raw_type);

Type* getStructType(Function* f);

unsigned getFieldIndex(Type* struct_type);

set<string> collectUnaryPredicate(Function* F);

set<string> collectAllPredicates(Function* F);

map<string, int> covertSetMap(Function* F, set<string>& st, int& counter);

bool doInvokeMemory(Value* val, unsigned nfield);

void printMap(map<string, int>& mp);

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

template <typename T>
bool isSubVector(vector<T>& vec, vector<T>& sub)
{
	for (T& s : sub) {
		if (!isVectorElmt(vec, s)) { return false; }
	}
	return true;
}

template <typename T>
bool isSameVector(vector<T>& vec1, vector<T>& vec2)
{
	return isSubVector(vec1, vec2) && isSubVector(vec2, vec1);
}

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