SLLCheck: 
	make SLLCompile
	make runPass

SLLCompile:
	make clean
	make all

TESTPATH = ./Testcase
TESTNAME = insert

TESTSRC = $(TESTPATH)/$(TESTNAME).c
TESTIR = $(TESTPATH)/$(TESTNAME).ll

runPass: SLLCheck.cpp
	clang -emit-llvm $(TESTSRC) -S -o $(TESTIR)
	opt -mem2reg -instnamer $(TESTIR) -S -o $(TESTIR)
	opt -load ./SLLCheck.so -sll-check $(TESTIR) -o out

CLANGFLAGS = clang++ -std=c++14 -fPIC -c


Utility.o: ./Utility/Utility.h ./Utility/Utility.cpp
	$(CLANGFLAGS) ./Utility/Utility.cpp

Update.o: ./ControlFlow/Update/Update.h ./ControlFlow/Update/Update.cpp
	$(CLANGFLAGS) ./ControlFlow/Update/Update.cpp

Focus.o: ./ControlFlow/Focus/Focus.h ./ControlFlow/Focus/Focus.cpp
	$(CLANGFLAGS) ./ControlFlow/Focus/Focus.cpp

ShapeStructure.o: ./ShapeStructure/ShapeStructure.h ./ShapeStructure/ShapeStructure.cpp
	$(CLANGFLAGS) ./ShapeStructure/ShapeStructure.cpp


PredicateUtil.o: ./Utility/PredicateUtil.h ./Utility/PredicateUtil.cpp
	$(CLANGFLAGS) ./Utility/PredicateUtil.cpp

ParametricFramework.o: ./Framework/ParametricFramework.h ./Framework/ParametricFramework.cpp
	$(CLANGFLAGS) ./Framework/ParametricFramework.cpp

SLLAnalysis.o: ./Analysis/SLLAnalysis.h ./Analysis/SLLAnalysis.cpp
	$(CLANGFLAGS) ./Analysis/SLLAnalysis.cpp

all: SLLCheck.so

CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) -g -O2

%.so: %.o Utility.o ShapeStructure.o PredicateUtil.o ParametricFramework.o Focus.o Update.o SLLAnalysis.o 
	$(CXX) -dylib -fPIC -shared $^ -o $@

clean:
	rm -f *.o *~ *.so