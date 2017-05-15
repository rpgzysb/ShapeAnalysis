#ifndef __PREDICATE_ARG__
#define __PREDICATE_ARG__

#include <vector>
#include <string>

using namespace std;

// the arguments class
// just to make arguments look the same
// even though the number is different
class PredicateArg
{
public:
	PredicateArg(vector<string>& args_) : 
		args{ args_ }, 
		n_args{ static_cast<int>(args_.size()) } 
	{}
	PredicateArg() :
		 args{}, 
		 n_args{ 0 } 
	{}
	~PredicateArg() {}

	int getNumArgs() { return n_args; }
	bool isUnary() { return n_args == 1; }
	bool isBinary() { return n_args == 2; }
	vector<string> getArgs() { return args; }
	string getArg(int i) { return args[i]; }

private:
	vector<string> args;
	int n_args;
};

#endif