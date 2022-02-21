CXX=g++
CXXFLAGS=--std=c++11 -Wall -Wextra -Wpedantic -Wconversion
SRC=src/polem_enrich.cpp src/polem_enrich.h src/polem_query.cpp src/polem_query.h
PYTHON3=/usr/include/python3.6m
POLEM=/usr/local/include/polem-dev

.default: shared_lib

shared_lib: polem_enrich python3_polem_enrich
	$(CXX) $(CXXFLAGS) -lpython3.6m -lpolem-dev -shared bin/polem_enrich.o bin/polem_enrich_wrap.o bin/polem_query.o -o bin/_polem_enrich.so

polem_enrich: $(SRC)
	$(CXX) $(CXXFLAGS) -fpic -c src/polem_query.cpp -o bin/polem_query.o
	$(CXX) $(CXXFLAGS) -fpic -I$(POLEM) -c src/polem_enrich.cpp -o bin/polem_enrich.o

python3_polem_enrich: python3_wrapper
	$(CXX) $(CXXFLAGS) -fpic -I$(PYTHON3) -c src/polem_enrich_wrap.cxx -o bin/polem_enrich_wrap.o 

python3_wrapper: src/polem_enrich.i
	swig -c++ -python -py3 src/polem_enrich.i

move_files:
	cp src/polem_enrich.py ../rest-api/polem_enrich.py
	cp bin/_polem_enrich.so ../rest-api/_polem_enrich.so

