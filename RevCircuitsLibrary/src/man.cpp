/*
 * man.cpp
 *
 *  Created on: 5 cze 2014
 *      Author: Krzysztof
 */

#include <iostream>
#include <sstream>
#include <vector>
#include "reversible_gates.h"
#include "reversible_function_measures.h"
#include <cstdlib>
#include <ctime>

#include <cmath>

using namespace std;

rev_circuit* pso_epoch(int linesCount, rev_function* target_function);
int randomize_with_distribution(int* dist, int size);

int main(){
	int linesCount = 3;
	rev_gate_factory gate_factory(linesCount);
	rev_circuit* circuit = gate_factory.new_circuit();
	//circuit->gFactory = gate_factory;
	circuit->append("t1 a");
	circuit->append("t2 a,b");
	circuit->append("t2 b,c");
	circuit->append("t2 a,b");
	circuit->append("t2 a,c");
	circuit->append("t1 a");
	circuit->append("t2 a,c");
	circuit->append("t1 a");
	circuit->append("t2 a,b");
	cout<<circuit->str()<<endl<<"=================="<<endl;
	rev_circuit* circuit2 = optimize_circuit(circuit);
	cout<<circuit2->str()<<endl<<"=================="<<endl;
	rev_function* f = circuit->operate(rev_function::identity_function(linesCount));
	f->print_function();
	cout<<endl;
	f = circuit2->operate(rev_function::identity_function(linesCount));
	f->print_function();
}

int main2() {
	cout << "Hello World!!!" << endl; // prints Hello World!!!
	int size = 4;
	bool tab[size];
	//for (int in = 2; in<8; in++)
	int a[] = {0,1,2,3,4,5,6,7};
	rev_function* fun = new rev_function(3,a);
	cout<<":::"<<fun->permutationSize<<endl;

	for (int i=0; i<8;i++)
		cout<<fun->permutation[i]<<" ";
	cout<<endl;
	int clines[] = {1};
	rev_gate_factory gate_factory(3);
	rev_gate* tgate = gate_factory.new_rev_gate("t2 c,a");//gate_factory.new_cnot_gate(3,1); gate_factory.new_cnot_gate(3,1);
	rev_gate* tgate2 = gate_factory.new_rev_gate("t2 b,a"); //new toffoli_gate(3,2,1,clines);
	cout<<tgate->str()<<endl;
	rev_function* f1= tgate->operate(fun);
	cout<<tgate2->str()<<endl;
	rev_function* f2= tgate2->operate(f1);
	//f2->print_function();
	//fun->print_function();

	rev_function_distance* hamm = rev_function_measures_factory::hamming_measure(fun,f2);
	cout<<"---------------------------"<<endl;
	cout<<"f1\t";
	fun->print_function();
	cout<<"f2\t";
	f2->print_function();
	cout<<endl;
	cout<<"Hamm distance:"<<hamm->overal_distance<<endl;
		for (int i=0; i<hamm->linesCount;i++)
			cout<<i<<"::"<<hamm->lines_distance[i]<<endl;

	cout<<"Original name"<<tgate->str()<<endl;
	cout<<"Copy name"<<tgate->copy()->str();

	//test rev circuit
	rev_circuit circ(tgate->linesCount);
	circ.append(tgate->copy());
	circ.append(tgate2->copy());
	rev_function* f3 = circ.operate(fun);
	cout<<"Circuits_tests"<<endl;
	f2->print_function();
	f3->print_function();
	delete hamm;
	hamm = rev_function_measures_factory::hamming_measure(f2,f3);
	cout<<"distance: "<<hamm->overal_distance<<endl;


	string str = "t2 c, a";
	rev_gate* g = gate_factory.new_rev_gate(str);
	cout<<g->str();



	delete fun;
	delete f2;
	delete f1;
	delete tgate;

	int linesCount = 3;
	//int perm[]={0,1,3,2,4,5,6,7};//
	int perm[]= {1,3,4,2,5,7,6,0};
	int hwb4[] = {0,2,4,12,8,5,9,11,1,6,10,13,3,14,7,15};
	rev_function* target_function = new rev_function(linesCount,perm);
	linesCount = 4;
	target_function = new rev_function(linesCount,hwb4);
	rev_circuit* optimal = NULL;
	rev_circuit* worst = NULL;
	for (int i=0; i<1000; i++){
		bool coppied = false;
		rev_circuit* circuit =pso_epoch(linesCount,target_function);
		if (optimal==NULL) {
			optimal=circuit;
			worst = circuit;
			coppied = true;
		}
		if (optimal->size()> circuit->size()) {
			optimal=circuit;
			coppied = true;
		}
		if (worst->size()<circuit->size()){
			worst = circuit;
			coppied = true;
		}
		if (!coppied) delete circuit;
		if (i%10==0) cout<<i<<endl;
	}
		cout<<endl;
		if (optimal->size() <20 )cout<<optimal->str();
		cout<<endl<<optimal->size()<<" gates used for optimal";
		f2 = optimal->operate(rev_function::identity_function(linesCount));
		cout<<endl;
		cout<<endl;
		target_function->print_function();
		cout<<endl<<worst->size()<<" gates used for worst";
		f2 = worst->operate(rev_function::identity_function(linesCount));
		cout<<endl;
		f2->print_function();



	/*rev_function* state = rev_function::identity_function(linesCount);
	rev_gate_factory gFactory = rev_gate_factory(linesCount);
	pso_particle* particle = new pso_particle(1);
	//particle->position = new rev_particle_position(linesCount);
	rev_circuit* circuit = new rev_circuit(linesCount);

	srand (time(NULL));
	delete hamm;
	hamm = rev_function_measures_factory::hamming_measure(state,target_function);

	cout<<endl<<"Function to do:"<<endl;
	target_function->print_function();
	cout<<"Function to start with:"<<endl;
	state->print_function();
	cout<<"distance: "<<hamm->overal_distance<<endl;



	int iterations =0;
	rev_function* temp = NULL;
	int opLine = -1;
	bool result = false;
	rev_gate* nGate;
	rev_gate* prevGate =NULL;
	int prev_distance = hamm->overal_distance;
	rev_function_distance* temp_distance = NULL;
	int empty_iterations = 0;
	while(hamm->overal_distance>0){
			opLine=-1;
			while (opLine<0){
				opLine=1+ (rand()%linesCount);
				if (empty_iterations<5 && hamm->lines_distance[opLine-1] ==0) opLine =-1;
			}
			int controlLines = (rand()%(linesCount));
			set<int> lines;
			lines.insert(opLine);
			//cout<<endl<<"oper line: " <<opLine<<endl<<"control Lines ["<<controlLines<<"] ::";
			for (int i=0; i<controlLines; i++)
			{
				int l = 1+ (rand()%linesCount);
				if (lines.insert(l).second==false)
				{	//L already existed in lines set
					i=i-1;
				}
				//else cout<<" "<<l;
			}
			lines.erase(opLine);
			nGate = gFactory.new_toffoli_gate(opLine,lines);
			if (prevGate!=NULL && prevGate->name == nGate->name){
				//cout<<"The same gate: "<<prevGate->name<<" :: "<<nGate->name<<endl;
				delete nGate;
			}
			else{
				temp = nGate->operate(state);
				temp_distance = rev_function_measures_factory::hamming_measure(temp,target_function);
				if (abs(prev_distance - temp_distance->overal_distance)<5){
					circuit->append( nGate );
					//cout<<circuit->str()<<endl<<"=================="<<endl;
					prevGate = nGate;
					delete state;
					state = temp;
					delete hamm;
					hamm = temp_distance;
					prev_distance = hamm->overal_distance;
					empty_iterations = 0;
				}
				else{
					delete temp;
					delete temp_distance;
					empty_iterations ++;
				}
			}
			iterations++;
	}
	if (hamm->overal_distance==0) {
			result=true;
			hamm->f1->print_function();
			hamm->f2->print_function();
	}
	cout<<endl;
	cout<<"After "<<iterations<<" iterations"<<endl;
	if (circuit->size() <10 )cout<<circuit->str();
	cout<<endl<<circuit->size()<<" gates used";
	f2 = circuit->operate(rev_function::identity_function(linesCount));
	cout<<endl;
	f2->print_function();
	cout<<endl;
	target_function->print_function();

	/*cout<<"New Approach"<<endl;
	rev_circuit* circ2 = new rev_circuit(linesCount);
	circ2->append(gFactory.new_rev_gate("t1 c"));
	circ2->append(gFactory.new_rev_gate("t3 a,c,b"));
	circ2->append(gFactory.new_rev_gate("t1 c"));
	cout<<circ2->str()<<endl;
	f2=circ2->operate(rev_function::identity_function(linesCount));
	f2->print_function();
	target_function->print_function();
	*/

		return 0;
}

rev_circuit* pso_epoch(int linesCount, rev_function* target_function){
		srand (time(NULL));
		rev_function* state = rev_function::identity_function(linesCount);;
		rev_gate_factory gFactory = rev_gate_factory(linesCount);;
		rev_function_distance* hamm = rev_function_measures_factory::hamming_measure(state,target_function);
		rev_circuit* circuit = new rev_circuit(linesCount);
		int iterations =0;
		rev_function* temp = NULL;
		int opLine = -1;
		rev_gate* nGate;
		rev_gate* prevGate =NULL;
		int prev_distance = hamm->overal_distance;
		rev_function_distance* temp_distance = NULL;
		int empty_iterations = 0;
		while(hamm->overal_distance>0){

				opLine = 1+ randomize_with_distribution(hamm->lines_distance,linesCount);
				int controlLines = (rand()%(linesCount));
				set<int> lines;
				lines.insert(opLine);
				//cout<<endl<<"oper line: " <<opLine<<endl<<"control Lines ["<<controlLines<<"] ::"
				for (int i=0; i<controlLines; i++)
				{
					int l = 1+ (rand()%linesCount);
					if (lines.insert(l).second==false)
					{	//l already existed in lines set
						i=i-1;
					}
					//else cout<<" "<<l;
				}
				lines.erase(opLine);
				nGate = gFactory.new_toffoli_gate(opLine,lines);
				if (prevGate!=NULL && prevGate->name == nGate->name){
					//cout<<"The same gate: "<<prevGate->name<<" :: "<<nGate->name<<endl;
					delete nGate;
				}
				else{
					temp = nGate->operate(state);
					temp_distance = rev_function_measures_factory::hamming_measure(temp,target_function);
					if (abs(prev_distance - temp_distance->overal_distance)<5){
						circuit->append( nGate );
						//cout<<circuit->str()<<endl<<"=================="<<endl;
						prevGate = nGate;
						delete state;
						state = temp;
						delete hamm;
						hamm = temp_distance;
						prev_distance = hamm->overal_distance;
						empty_iterations = 0;
					}
					else{
						delete temp;
						delete temp_distance;
						empty_iterations ++;
					}
				}
				iterations++;
		}
		return circuit;
}

int randomize_with_distribution(int* dist, int size){
	int x, y;
	bool accept = false;
	int sum = 0;
	for (int i=0; i<size; i++){
		if (dist[i]>sum) sum=dist[i];
	}
	while (!accept){
		x = rand()%size;
		y = rand()%sum+1;
		if (dist[x]>=y) accept = true;
	}
	return x;
}


