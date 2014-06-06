/*
 * reversible_gates.h
 *
 *  Created on: 18 mar 2014
 *      Author: KPodlaski
 */
#ifndef REVERSIBLE_GATES_H_
#define REVERSIBLE_GATES_H_

#include <string>
#include <vector>
#include <set>
#include <sstream>

using namespace std;

void int_to_bites_array(int in, int numberOfBites, bool* out);
class rev_gate_factory;

struct rev_function{
	rev_function(int linesCount, int* permutation);
	int* permutation;
	int permutationSize;
	int linesCount;
	static rev_function* identity_function(int numberOfLines);
	void print_function();
	~rev_function();
};



enum gate_types{
	CIRCUIT = 0,
	NOT = 1,
	CNOT = 2,
	TOFFOLI = 3,
	NEG_TOFFOLI = 4,
	MIX_TOFFOLI = 5,
	FREDKIN = 6
};


class rev_gate{
public:
	rev_gate(int linesCount, string name);
	rev_gate(int linesCount);
	string name;
	int linesCount;
	gate_types gate_type;
	virtual rev_function* operate(rev_function * input) =0;
	virtual string str()=0;
	virtual rev_gate* copy()=0;
};

class rev_circuit: public rev_gate{
public :
	virtual rev_function* operate(rev_function * input);
	virtual string str();
	rev_circuit(int linesCount);
	virtual ~rev_circuit();
	virtual rev_circuit* copy();
	virtual int size();
	virtual void addLeft(rev_gate* gate);
	virtual void addRight(rev_gate* gate);
	virtual void append(rev_gate* gate);
	virtual void append(rev_circuit* circuit);
	virtual void addLeft(string gate);
	virtual void addRight(string gate);
	virtual void append(string gate);
	vector<rev_gate*> gates;
	static void set_gates_factory(rev_gate_factory* gFactory);

private:
	stringstream sname;
	static rev_gate_factory* gFactory;

friend class rev_gate_factory;
};

class toffoli_gate : public rev_gate{
public:
	int operationLine;
	vector<int> controlLines;

	virtual rev_function* operate(rev_function * input);
	virtual string str();
	toffoli_gate(int linesCount, int operationLine,int controlLinesCount, int* controlLines);
	virtual toffoli_gate* copy();
private:
	toffoli_gate(int linesCount, int operationLine,vector<int> controlLines);
	int operateOnSignal(int signal);
	toffoli_gate(int linesCount);

friend class rev_gate_factory;
};

class rev_gate_factory{
private:
	int linesCount;
public:
	rev_gate_factory(int linesCount);
	rev_gate* new_not_gate(int operationLine);
	rev_gate* new_cnot_gate(int operationLine, int controlLine);
	rev_gate* new_toffoli_gate(int operationLine,int controlLinesCount, int* controlLines);
	rev_gate* new_toffoli_gate(int operationLine,set<int> controlLines);
	rev_gate* new_toffoli_gate(int operationLine,vector<int> controlLines);
	rev_gate* new_rev_gate(string gate_as_string);
	rev_circuit* new_circuit();
private:
	rev_gate* new_toffoli_gate(string sgate);
	int line_name_to_line_id(string name);
};

rev_circuit* optimize_circuit(rev_circuit* circuit);

#endif /* REVERSIBLE_GATES_H_ */
