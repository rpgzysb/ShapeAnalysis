
#include "Utility.h"

/*
 * Header file for utility.
 * Mainly for shape structures.
*/

using namespace std;
using namespace llvm;

// determine whether ptr_type is a pointer of raw_type
bool isPointerOf(Type* ptr_type, Type* raw_type)
{
	// need to be the pointer type
	if (ptr_type->isPointerTy() && (ptr_type->getNumContainedTypes() == 1)) {
		// get the underlying type
		Type* contained_type = ptr_type->getContainedType(0);
		return contained_type == raw_type;
	}
	return false;
}

// determine whether ptr_type is a double pointer of raw_type
bool isDoublePointerOf(Type* ptr_type, Type* raw_type)
{
	// need to be the pointer type
	if (ptr_type->isPointerTy() && (ptr_type->getNumContainedTypes() == 1)) {
		// get the underlying type
		Type* contained_type = ptr_type->getContainedType(0);
		return isPointerOf(contained_type, raw_type);
	}
	return false;
}

// get the data structure type in LLVM
Type* getStructType(Function* f)
{
	// pointers need to be stored in unordered set
	unordered_set<Value*> vals{};
	// collect all possible variables in the function
	for (auto B = f->begin(); B != f->end(); ++B) {
		for (auto I = B->begin(); I != B->end(); ++I) {
			for (auto OI = I->op_begin(); OI != I->op_end(); ++OI) {
				// iterate thourgh all operands
				Value* val = *OI;
				if (isa<Instruction>(val) || isa<Argument>(val)) {
					// instructions && arguments
					vals.insert(val);
				}
			}
		}
	}

	// iterate through all the variables
	// and see if their type is self reference
	// like linked list
	for (Value* val : vals) {
		// type of the current variable
		Type* val_type = val->getType();
		if (val_type->isPointerTy()) {
			unsigned num_contained_types{ val_type->getNumContainedTypes() };
			if (num_contained_types == 1) {
				// get the underlying type
				Type* contained_types = val_type->getContainedType(0);
				if (contained_types->isStructTy()) {
					return contained_types;
				}
			}
		}
	}

	return nullptr;
}

// get the field index of the next pointer in LLVM
unsigned getFieldIndex(Type* struct_type)
{
	unsigned num_fields{ struct_type->getNumContainedTypes() };
	for (unsigned i = 0; i < num_fields; ++i) {
		Type* member_type = struct_type->getContainedType(i);
		if (isPointerOf(member_type, struct_type)) {
			return i;
		}
	}
	return numeric_limits<unsigned>::max();
}

// collect the name of all unary predicates
set<string> collectUnaryPredicate(Function* F)
{
	set<string> unaryValues{};
	// get struct type
	// unary predicates are pointer of struct type
	// like List*
	Type* struct_type = getStructType(F);
	for (auto B = F->begin(); B != F->end(); ++B) {
		for (auto I = B->begin(); I != B->end(); ++I) {
			for (auto OI = I->op_begin(); OI != I->op_end(); ++OI) {
				Value* val = *OI;
				if (isa<Instruction>(val) || isa<Argument>(val)) {
					// find all variables
					Type* oprnd_type = val->getType();
					if (isPointerOf(oprnd_type, struct_type)) {
						// is a pointer of struct type
						unaryValues.insert(val->getName().str());
					}
				}
			}
		}
	}

	// special predicates
	unaryValues.insert("_sm");
	unaryValues.insert("_isNew");

	return move(unaryValues);
}

// collect the name of all predicates
set<string> collectAllPredicates(Function* F)
{
	set<string> preds{ collectUnaryPredicate(F) };
	// only has one in this implementation
	preds.insert("_n");

	return move(preds);
}

// convert the set of names into a map 
map<string, int> covertSetMap(Function* F, set<string>& st, int& counter)
{
	map<string, int> mps{};
	for (string pred : st) {
		// counter will be updated
		mps[pred] = counter;
		counter++;
	}
	return move(mps);
}

// see if the current instruction val involves memory operations
bool doInvokeMemory(Value* val, unsigned nfield)
{
	// only consider the getelementptr instruction
	if (GetElementPtrInst* gepi = dyn_cast<GetElementPtrInst>(val)) {
		Use* last_ref = gepi->idx_end();
		--last_ref;
		Value* deref_idx = last_ref->get();
		// get the last index
		if (ConstantInt* ci = dyn_cast<ConstantInt>(deref_idx)) {
			unsigned num = ci->getZExtValue();
			// see if the dereference is index 
			// is the next field index
			return num == nfield;
		}
	}
	return false;
}

// helper function to print the map
void printMap(map<string, int>& mp)
{
	for (auto kvp : mp) {
		outs() << kvp.first << ": " << kvp.second << "\n";	
	}
}

