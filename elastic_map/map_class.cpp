#include "map_class.h"
#include <iostream>
#include <cmath>

#define MIN(a,b) (a<b?a:b)

double eDist(vector<double> a, vector<double> b)
{
	double d = 0.0;
	for (int i = 0; i < MIN(a.size(), b.size()); i++)
	{
		double t = a[i] - b[i];
		d += t * t;
	}
	return sqrt(d);
}

//TODO: Interpolate here
vector<double> ElasticMap::getOut(vector<double> in)
{
	double minDist = -1.0;
	vector<double> out;
	for (int i = 0; i < nodes.size(); i++)
	{
		double d = eDist(in, nodes[i].i);
		if (d < minDist || minDist < 0.0)
		{
			minDist = d;
			out = nodes[i].o;
		}
	}

	return out;
}

ElasticMap::ElasticMap(unsigned int dim, unsigned int nn, double min, double max)
{
	uniform_real_distribution<double> unif(min, max);

	for (int i = 0; i < nn; i++)
	{
		EM_Node newN;
		for (int j = 0; j < dim; j++)
		{
			double v = unif(rng);
			newN.i.push_back(v);
			newN.o.push_back(v);
		}

		nodes.push_back(newN);
	}
}

void ElasticMap::train(vector<dat> data)
{

}

vector<oDat> ElasticMap::output(vector<iDat> in)
{
	vector<oDat> out;

	for (int i = 0; i < in.size(); i++)
	{
		vector<double> tempO = getOut(in[i].iVec);
		oDat d;
		d.s = in[i].s;
		d.t = in[i].t;
		d.oVec = tempO;
		out.push_back(d);
	}

	return out;
}

