
/*
 * Implementation of the shape structure.
*/


#include "ShapeStructure.h"

using namespace std;
using namespace llvm;

// normal constructor
ShapeStructure::ShapeStructure(map<string, int>& indivs, 
	shared_ptr<map<string, int>> preds, 
	map<int, map<int, int>> unaryPredicates_,
	map<int, 
				map<int, map<int, int>>
			> binaryPredicates_) :
	individuals{ indivs },
	predicates{ preds }
{
	// the count begins from unused numbers
	indiv_count = individuals.size() + predicates->size();

	// add into the unary predicates
	for (auto indivp : indivs) {
		individualsRange.insert(indivp.first);
		for (auto kvp : *preds) {
			unaryPredicates[kvp.second][indivp.second] = 0;
		}
	}

	// add into the binary predicates
	for (auto indivp : indivs) {
		for (auto indivp2 : indivs) {
			binaryPredicates[7][indivp.second][indivp2.second] = 0;
		}
	}

	// initialize according to the argument
	for (auto kvp : unaryPredicates_) {
		for (auto kvp2 : kvp.second) {
			unaryPredicates[kvp.first][kvp2.first] = kvp2.second;
		}
	}

	for (auto kvp : binaryPredicates_) {
		for (auto kvp2 : kvp.second) {
			for (auto kvp3 : kvp2.second) {
				binaryPredicates[kvp.first][kvp2.first][kvp3.first] = kvp3.second;
			}
		}
	}

}

// copy constructor
ShapeStructure::ShapeStructure(const ShapeStructure& ss) :
	individuals{ ss.getIndividuals() },
	predicates{ ss.getPredicatePtr() },
	unaryPredicates{ ss.getUnaryPredicates() },
	binaryPredicates{ ss.getBinaryPredicates() },
	indiv_count{ ss.indiv_count }, 
	individualsRange{ ss.getIndividualsRange() }
	{}

// hash the index
int ShapeStructure::getIndividualIdx(string indiv)
{
	return individuals[indiv];
}

int ShapeStructure::getPredicateIdx(string pred)
{
	return (*predicates)[pred];
}

// get the shape structure
int ShapeStructure::getUnaryLogic(string pred, string indiv)
{
	int ival = getIndividualIdx(indiv);
	int pval = getPredicateIdx(pred);

	return unaryPredicates[pval][ival];
}

int ShapeStructure::getBinaryLogic(string binary, string indiv1, string indiv2)
{
	// TODO: binary predicate hash
	int bval = 7; // next field
	int ival1 = getIndividualIdx(indiv1);
	int ival2 = getIndividualIdx(indiv2);
	return binaryPredicates[bval][ival1][ival2];
}

void ShapeStructure::createIndividual()
{
	// create new individuals on malloc
	string new_name{ "_malloc_" + to_string(indiv_count) };
	individuals[new_name] = indiv_count;
	individualsRange.insert(new_name);

	// create slots
	for (auto& kvp : unaryPredicates) {
		int pred{kvp.first};
		unaryPredicates[pred][indiv_count] = 0;
	}

	map<string, int> predicateMap = *predicates;
	int n_index = predicateMap["_n"];

	for (auto& kvp : binaryPredicates[n_index]) {
		int v1{kvp.first};
		binaryPredicates[n_index][v1][indiv_count] = 0;
		binaryPredicates[n_index][indiv_count][v1] = 0;
	}
	binaryPredicates[n_index][indiv_count][indiv_count] = 0;

	// update
	unaryPredicates[predicateMap["_isNew"]][indiv_count] = 2;

	indiv_count++;

	outs() << "create individual " << new_name << "\n";
}

// update the shape structure
void ShapeStructure::setUnaryPredicate(string pred, string indiv, int val)
{
	int ival = getIndividualIdx(indiv);
	int pval = getPredicateIdx(pred);
	unaryPredicates[pval][ival] = val;
}

void ShapeStructure::setBinaryPredicate(string binary, 
	string indiv1, string indiv2, int val)
{
	// TODO: binary predicate hash
	int bval = 7; // next field
	int ival1 = getIndividualIdx(indiv1);
	int ival2 = getIndividualIdx(indiv2);
	binaryPredicates[bval][ival1][ival2] = val;
}

// see if two shape structures are isomorphic
bool ShapeStructure::operator==(const ShapeStructure& ss)
{
	return (individuals == ss.getIndividuals()) &&
		(unaryPredicates == ss.getUnaryPredicates()) &&
		(binaryPredicates == ss.getBinaryPredicates());
}
