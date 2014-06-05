/*
 * reversible_function_measures.h
 *
 *  Created on: 31 mar 2014
 *      Author: KPodlaski
 */

#ifndef REVERSIBLE_FUNCTION_MEASURES_H_
#define REVERSIBLE_FUNCTION_MEASURES_H_

#include "reversible_gates.h"
#include <string>

class rev_function_measures_factory;

class rev_function_distance{
public:
	string name;
	int linesCount;
	int* lines_distance;
	int overal_distance;
	virtual ~rev_function_distance();
	rev_function *f1, *f2;
protected:
	rev_function_distance(rev_function * f1, rev_function * f2);

	virtual void countDistances()=0;
friend class rev_function_measures_factory;
};

class rev_function_hamming_distance: public rev_function_distance{
public:
	virtual ~rev_function_hamming_distance();
private:
	rev_function_hamming_distance(rev_function * f1, rev_function * f2);
	virtual void countDistances();
friend class rev_function_measures_factory;
};

class rev_function_measures_factory{
public:
	static rev_function_distance* hamming_measure(rev_function * f1, rev_function * f2);
};

class rev_state_functions_distance_behaviour{
public:
	rev_circuit* circuit;
	vector<rev_function> state_functions;
	vector<rev_function_distance> state_distance;

private:
};

#endif /* REVERSIBLE_FUNCTION_MEASURES_H_ */
