#ifndef MAP_CLASS_H_
#define MAP_CLASS_H_

#include "loadTraining.h"
#include <random>

struct EM_Node
{
	vector<double> i;
	vector<double> o;
	vector<double> e;
};

struct Cluster
{
	vector<double> centroid;
	vector<EM_Node*> nodes;
	bool dead;
};

class ElasticMap
{
private:
	std::mt19937_64 rng;
	vector<EM_Node> nodes;
	vector<Cluster> k_means;
	vector<double> getOut(vector<double>);
	Cluster* getCCluster(vector<double>);
	EM_Node* getCNode(vector<double>);
public:
	ElasticMap(unsigned int, unsigned int, double);
	void train(vector<dat>);
	vector<oDat> output(vector<iDat>);
};

#endif
