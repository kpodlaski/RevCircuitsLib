/*
 * reversible_function_measures.cpp
 *
 *  Created on: 31 mar 2014
 *      Author: KPodlaski
 */

#include "reversible_function_measures.h"
#include "reversible_gates.h"
#include <algorithm>

using namespace std;

rev_function_distance::rev_function_distance(rev_function* _f1, rev_function* _f2)  : f1(_f1), f2(_f2){
	linesCount = f1->linesCount;
	lines_distance = new int[linesCount];
	overal_distance = -1;
	fill( lines_distance, lines_distance + linesCount, 0 );
}

rev_function_distance::~rev_function_distance(){
	delete [] lines_distance;
	f1=NULL;
	f2=NULL;
}
rev_function_hamming_distance::rev_function_hamming_distance(rev_function* _f1, rev_function* _f2) : rev_function_distance(_f1,_f2){

}

rev_function_hamming_distance::~rev_function_hamming_distance(){
}


void rev_function_hamming_distance::countDistances(){
	overal_distance = 0;
	int temp;
	for (int i=0; i<f1->permutationSize; i++){
		temp = f1->permutation[i] ^ f2->permutation[i];
		for (int k=0; k<linesCount; k++){
			if (temp%2==1){
				lines_distance[k] += 1;
				overal_distance+=1;
			}
			temp=temp>>1;
		}
	}
}


rev_function_distance* rev_function_measures_factory::hamming_measure(rev_function * f1, rev_function * f2){
	rev_function_distance* measure = new rev_function_hamming_distance(f1,f2);
	measure->countDistances();
	return measure;
}


