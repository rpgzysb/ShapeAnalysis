
#include "Utility.h"

using namespace std;
using namespace llvm;

bool isPointerOf(Type* ptr_type, Type* raw_type)
{
	if (ptr_type->isPointerTy() && (ptr_type->getNumContainedTypes() == 1)) {
		Type* contained_type = ptr_type->getContainedType(0);
		return contained_type == raw_type;
	}
	return false;
}

bool isDoublePointerOf(Type* ptr_type, Type* raw_type)
{
	if (ptr_type->isPointerTy() && (ptr_type->getNumContainedTypes() == 1)) {
		Type* contained_type = ptr_type->getContainedType(0);
		return isPointerOf(contained_type, raw_type);
	}
	return false;
}

Type* getStructType(Function* f)
{
	unordered_set<Value*> vals{};
	for (auto B = f->begin(); B != f->end(); ++B) {
		for (auto I = B->begin(); I != B->end(); ++I) {
			for (auto OI = I->op_begin(); OI != I->op_end(); ++OI) {
				Value* val = *OI;
				if (isa<Instruction>(val) || isa<Argument>(val)) {
					vals.insert(val);
				}
			}
		}
	}

	for (Value* val : vals) {
		Type* val_type = val->getType();
		if (val_type->isPointerTy()) {
			unsigned num_contained_types{ val_type->getNumContainedTypes() };
			if (num_contained_types == 1) {
				Type* contained_types = val_type->getContainedType(0);
				if (contained_types->isStructTy()) {
					return contained_types;
				}
			}
		}
	}

	return nullptr;
}

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

set<string> collectUnaryPredicate(Function* F)
{
	set<string> unaryValues{};
	Type* struct_type = getStructType(F);
	for (auto B = F->begin(); B != F->end(); ++B) {
		for (auto I = B->begin(); I != B->end(); ++I) {
			for (auto OI = I->op_begin(); OI != I->op_end(); ++OI) {
				Value* val = *OI;
				if (isa<Instruction>(val) || isa<Argument>(val)) {
					Type* oprnd_type = val->getType();
					if (isPointerOf(oprnd_type, struct_type)) {
						unaryValues.insert(val->getName().str());
					}
				}
			}
		}
	}
	unaryValues.insert("_sm");
	unaryValues.insert("_isNew");

	return move(unaryValues);
}

set<string> collectAllPredicates(Function* F)
{
	set<string> preds{ collectUnaryPredicate(F) };
	
	preds.insert("_n");

	return move(preds);
}

map<string, int> covertSetMap(Function* F, set<string>& st, int& counter)
{
	map<string, int> mps{};
	for (string pred : st) {
		mps[pred] = counter;
		counter++;
	}
	return move(mps);
}

bool doInvokeMemory(Value* val, unsigned nfield)
{
	if (GetElementPtrInst* gepi = dyn_cast<GetElementPtrInst>(val)) {
		Use* last_ref = gepi->idx_end();
		--last_ref;
		Value* deref_idx = last_ref->get();
		if (ConstantInt* ci = dyn_cast<ConstantInt>(deref_idx)) {
			unsigned num = ci->getZExtValue();
			return num == nfield;
		}
	}
	return false;
}

void printMap(map<string, int>& mp)
{
	for (auto kvp : mp) {
		outs() << kvp.first << ": " << kvp.second << "\n";	
	}
}

