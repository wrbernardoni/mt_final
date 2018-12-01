#ifndef MAP_CLASS_H_
#define MAP_CLASS_H_

#include "loadTraining.h"

struct EM_Node
{
	vector<double> i;
	vector<double> o;
};

class ElasticMap
{
private:

public:
	void train(vector<dat>);
	vector<oDat> output(vector<dat>);
};

#endif
