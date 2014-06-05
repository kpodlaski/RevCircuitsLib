/*
 * reversible_gates.cpp
 *
 *  Created on: 27 mar 2014
 *      Author: KPodlaski
 */

#include "reversible_gates.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>
#include <cstring>

using namespace std;

rev_function::rev_function(int linesCount, int* permut) : linesCount(linesCount){
	permutationSize = 1<<linesCount;
	permutation = new int[permutationSize];
	memcpy(permutation,permut,permutationSize* sizeof(int));
}

rev_function::~rev_function(){
	delete [] permutation;
}


void rev_function::print_function(){
	stringstream ss;
	for (int i=0; i<permutationSize;i++)
			ss<<permutation[i]<<" ";
	cout<<ss.str()<<endl;
}


rev_function* rev_function::identity_function(int numberOfLines){
	int maxValue = 1<<numberOfLines;
	int perm[maxValue];
	for (int i=0; i<maxValue; i++)
		perm[i]=i;
	return new rev_function(numberOfLines,perm);
}

rev_gate::rev_gate(int linesCount, string name) : linesCount(linesCount), name(name) {
}


rev_gate::rev_gate(int linesCount) : linesCount(linesCount){}

rev_circuit::rev_circuit(int linesCount) : rev_gate(linesCount){
}

rev_function* rev_circuit::operate(rev_function* input){
	rev_function* out = input;
	rev_function* temp;
	for (int i=0; i<gates.size(); i++){
		temp = gates[i]->operate(out);
		if (out!=input) delete out;
		out = temp;
	}
	return out;
}

void rev_circuit::addRight(rev_gate* gate){
	gates.push_back(gate);
	sname<<gate->str()<<endl;
}

void rev_circuit::addRight(string gate){
	addRight(rev_circuit::gFactory->new_rev_gate(gate));
}


void rev_circuit::append(rev_gate* gate){
	addRight(gate);
}

void rev_circuit::append(rev_circuit* circuit){
	for (int i=0; i<circuit->gates.size(); i++)
	{
		append(circuit->gates[i]->copy());
	}

}

void rev_circuit::append(string gate){
	append(rev_circuit::gFactory->new_rev_gate(gate));
}



rev_circuit* rev_circuit::copy(){
	rev_circuit* result = new rev_circuit(linesCount);
	for (int i=0; i<gates.size(); i++){
		result->append(gates[i]->copy());
	}
	return result;
}

rev_circuit::~rev_circuit(){
	for (int i=0; i<gates.size(); i++){
		delete gates[i];
	}
	gates.clear();
}

stringstream& insert_at_beginning( stringstream& oss, const string& s )
  {
  streamsize pos = oss.tellp();
  oss.str( s + oss.str() );
  oss.seekp( pos + s.length() );
  return oss;
  }


void rev_circuit::addLeft(rev_gate* gate){
	gates.insert(gates.begin(),gate);
	insert_at_beginning(sname, gate->str());
}

void rev_circuit::addLeft(string gate){
	addLeft(rev_circuit::gFactory->new_rev_gate(gate));
}


string rev_circuit::str(){
	return sname.str();
}


int rev_circuit::size(){
	return gates.size();
}



toffoli_gate::toffoli_gate(int linesCount) : rev_gate(linesCount){
}


toffoli_gate::toffoli_gate(int linesCount, int operationLine, int controlLinesCount, int* controlLines) :
		rev_gate(linesCount), operationLine(operationLine-1){

	stringstream sname;
	sname<<"t"<<(controlLinesCount+1)<<" ";
	sname<<(char) ('a'+ operationLine-1);
	for (int i=0; i<controlLinesCount; i++){
		this->controlLines.push_back(controlLines[i]-1);
	}
	std::sort(this->controlLines.begin(), this->controlLines.end());
	for (int i=0; i<controlLinesCount; i++){
				sname<<","<<(char) ( 'a'+this->controlLines[i]);
	}
	switch (controlLinesCount+1){
		case NOT 	: 	gate_type = NOT;break;
		case CNOT 	:	gate_type = CNOT;break;
		default : gate_type = TOFFOLI;
	}
	name =sname.str();
}

toffoli_gate::toffoli_gate(int linesCount, int operationLine, vector<int> controlLines) :
		rev_gate(linesCount), operationLine(operationLine-1){
	int controlLinesCount = controlLines.size();
	stringstream sname;
	sname<<"t"<<(controlLinesCount+1)<<" ";
	sname<<(char) ('a'+ operationLine-1);
	this->controlLines = controlLines;
	std::sort(this->controlLines.begin(), this->controlLines.end());
	for (int i=0; i<controlLinesCount; i++){
				sname<<","<<(char) ( 'a'+this->controlLines[i]);
	}
	switch (controlLinesCount+1){
		case NOT 	: 	gate_type = NOT;break;
		case CNOT 	:	gate_type = CNOT;break;
		default : gate_type = TOFFOLI;
	}
	name =sname.str();
}



rev_function* toffoli_gate::operate(rev_function * in){
	rev_function* out = new rev_function(in->linesCount,in->permutation);
	for (int i=0; i<out->permutationSize; i++){
		out->permutation[i] = operateOnSignal(in->permutation[i]);
		//cout<<in->permutation[i] <<" --> "<<out->permutation[i]<<endl;
	}

	//out->print_function();
	return out;
}

int toffoli_gate::operateOnSignal(int in){
	int control = 1;
	for (int i=0; i<controlLines.size(); i++){
		control *= 	in & ( 1<<controlLines[i] );  //check true on selected control line
	}
	if (control>0){
		in^=(1<<operationLine); //toggle bit
	}
	return in;
}

string toffoli_gate::str(){
	return name;
}


toffoli_gate* toffoli_gate::copy(){
	toffoli_gate* copy = new toffoli_gate(linesCount);
	copy->operationLine = operationLine;
	copy->name = name;
	copy->controlLines = controlLines;
	return copy;
}

void int_to_bites_array(int in, int numberOfBites, bool* out){
	for (int i=0; i<numberOfBites; i++){
		out[i] =  in%2;
		in=in>>1;
	}
}



rev_gate_factory::rev_gate_factory(int _linesCount) : linesCount(_linesCount){
}

rev_circuit* rev_gate_factory::new_circuit(){
	rev_circuit* circ = new rev_circuit(linesCount);
	rev_circuit::gFactory = this;
	return circ;
}

rev_gate * rev_gate_factory::new_not_gate(int operationLine){
	return new toffoli_gate(linesCount, operationLine, 0, NULL);
}

rev_gate* rev_gate_factory::new_cnot_gate(int operationLine, int controlLine){
	int lines[]{controlLine};
	rev_gate * gate = new toffoli_gate(linesCount, operationLine, 1, lines);
	delete [] lines;
	return gate;
}

rev_gate* rev_gate_factory::new_toffoli_gate(int operationLine,int controlLinesCount, int* controlLines){
	return new toffoli_gate(linesCount, operationLine, controlLinesCount, controlLines);
}

rev_gate* rev_gate_factory::new_toffoli_gate(int operationLine,set<int> controlLines){
	vector<int> lines;
	set<int>::iterator it = controlLines.begin();
	for (it; it!=controlLines.end(); it++){
		lines.push_back( (*it) - 1 );
	}
	rev_gate* gate = new toffoli_gate(linesCount, operationLine, lines);
	return gate;
}

rev_gate* rev_gate_factory::new_toffoli_gate(int operationLine,vector<int> controlLines){
	vector<int> lines;
	for (int i=0; i<controlLines.size(); i++){
		lines.push_back(controlLines[i]+1);
	}
	rev_gate* gate = new toffoli_gate(linesCount, operationLine, lines);
	return gate;
}

rev_gate* rev_gate_factory::new_rev_gate(string sgate){
	switch (sgate[0]){
		case 't' : return new_toffoli_gate(sgate);
		//case 'f' : return new_fredkin_gate(sgate);
		default : return NULL;
	}
}

rev_gate* rev_gate_factory::new_toffoli_gate(string sgate){
	rev_gate* gate = NULL;
	size_t index = sgate.find_first_of(' ');
	size_t begin = 1;
	int gateLines = atoi(sgate.substr(begin,index).c_str()); //convert string to integer
	int line[gateLines];
	for (int i=0; i<gateLines; i++){
		begin = index+1;
		index = sgate.find_first_of(',',begin);
		line[i]= line_name_to_line_id(sgate.substr(begin,index));
		cout<<endl;
	}
	switch (gateLines){
		case 1 : gate = new_not_gate(line[0]); break;
		case 2 : gate = new_cnot_gate(line[0],line[1]); break;
		default : gate = new_toffoli_gate(line[0],gateLines-1,line+1);
	}
	return gate;
}

// trim from begin
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

int rev_gate_factory::line_name_to_line_id(string name){
	name = ltrim(rtrim(name));
	//cout<<"line::"<<name<<"======="<<(int) (name[0]-'a')+1<<endl;
	return (int) (name[0]-'a')+1;
}

// 'a' - sgate.substr(begin,index)[0];

//rev_gate* rev_gate_factory::cnot_gate(int operationLine, int controlLine);
//rev_gate* rev_gate_factory::toffoli_gate(int operationLine,int controlLinesCount, int* controlLines);

rev_circuit* optimize_circuit(rev_circuit* circuit){
	rev_circuit* result = new rev_circuit(circuit->linesCount);
	int lastOperationLine = -1;
	set<string> commuting_gates;
	for (vector<rev_gate*>::iterator gate; gate!=circuit->gates.end(); gate++){
		toffoli_gate* g = dynamic_cast<toffoli_gate*> (*gate);
		if (g->operationLine == lastOperationLine) {
			if (commuting_gates.insert(g->name).second==true){
				result->append(g);
			}
		}
		else{
			commuting_gates.clear();
			lastOperationLine = g->operationLine;
			result->append(g);
			commuting_gates.insert(g->name);
		}

	}
	return result;
}
