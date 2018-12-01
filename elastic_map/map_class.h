#ifndef MAP_CLASS_H_
#define MAP_CLASS_H_

#include "loadTraining.h"
#include <random>

struct EM_Node
{
	vector<double> i;
	vector<double> o;
};

class ElasticMap
{
private:
	std::mt19937_64 rng;
	vector<EM_Node> nodes;
	vector<double> getOut(vector<double>);
public:
	ElasticMap(unsigned int, unsigned int, double, double);
	void train(vector<dat>);
	vector<oDat> output(vector<iDat>);
};

#endif
