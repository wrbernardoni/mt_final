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

double norm(vector<double> a)
{
	double d = 0.0;
	for (int i = 0; i < a.size(); i++)
	{
		d += a[i] * a[i];
	}
	return sqrt(d);
}

Cluster* ElasticMap::getCCluster(vector<double> in)
{
	double minDist = -1.0;
	int clusterI = 0;
	vector<double> out;
	for (int i = 0; i < k_means.size(); i++)
	{
		if (k_means[i].dead)
			continue;

		double d = eDist(in, k_means[i].centroid);
		if (d < minDist || minDist < 0.0)
		{
			minDist = d;
			clusterI = i;
		}
	}

	return &k_means[clusterI];
}

EM_Node* ElasticMap::getCNode(vector<double> in)
{
	Cluster* c = getCCluster(in);
	double minDist = -1.0;
	EM_Node* node = NULL;

	minDist = -1.0;
	for (int i = 0; i < c->nodes.size(); i++)
	{
		double d = eDist(in, c->nodes[i]->i);
		if (d < minDist || minDist < 0.0)
		{
			minDist = d;
			node = c->nodes[i];
		}
	}

	return node;
}

//TODO: Interpolate and optimize here
vector<double> ElasticMap::getOut(vector<double> in)
{
	return getCNode(in)->o;
}

ElasticMap::ElasticMap(unsigned int dim, unsigned int nn, double sdev)
{
	normal_distribution<double> unif(0, sdev);

	cout << "Generating " << nn << " nodes" << endl;
	for (int i = 0; i < nn; i++)
	{
		EM_Node newN;
		for (int j = 0; j < dim; j++)
		{
			double v = unif(rng);
			newN.i.push_back(v);
			newN.o.push_back(v);
			newN.e.push_back(0);
		}

		nodes.push_back(newN);
	}

	cout << "Creating " << (int)(pow(10, (log(nodes.size()) / 3)) + 3) << " node clusters" << endl;
	uniform_int_distribution<int> choice(0, nodes.size());
	for(int i = 0; i < (pow(10, (log(nodes.size()) / 3)) + 1); i++)
	{
		Cluster c;
		c.dead = false;
		for (int j = 0; j < dim; j++)
		{
			c.centroid.push_back(unif(rng));
		}

		k_means.push_back(c);
	}

	for (int i = 0; i < nodes.size(); i++)
	{
		cout << "\tAllocating Clusters: " << (double)i / nodes.size() << "\r";
		if (i % 100 == 0)
			cout.flush();

		double mind = -1.0;
		int k_I = 0;
		for (int j = 0; j < k_means.size(); j++)
		{
			double d = eDist(k_means[j].centroid, nodes[i].i);
			if (d < mind || mind < 0)
			{
				mind = d;
				k_I = j;
			}
		}

		k_means[k_I].nodes.push_back(&nodes[i]);
	}

	cout << "\tAllocating Clusters: 1                 "<< endl;

	for (int i = 0; i < k_means.size(); i++)
	{
		cout << "\tRecomputing Centroids: " << (double)i / k_means.size() << "\r";
		if (i % 100 == 0)
			cout.flush();
		if (k_means[i].nodes.size() == 0)
		{
			k_means[i].dead = true;
			continue;
		}

		for (int j = 0; j < dim; j++)
		{
			double s = 0.0;
			for (int k = 0; k < k_means[i].nodes.size(); k++)
			{
				s += k_means[i].nodes[k]->i[j];
			}

			k_means[i].centroid[j] = s / (double)k_means[i].nodes.size();
		}
	}
	cout << "\tRecomputing Centroids: 1                       " << endl;
}

struct datToNode
{
	dat d;
	EM_Node* n;
};

struct rib
{
	EM_Node* a;
	EM_Node* b;
	EM_Node* c;
};

struct band
{
	EM_Node* a;
	EM_Node* b;
};

double computePs(vector<datToNode> in)
{
	double s = 0.0;

	for (int i = 0; i < in.size(); i++)
	{
		s += pow(eDist(in[i].d.tVec, in[i].n->o),2);
	}

	return 0.5 * s;
}

double computePe(vector<band> in)
{
	double s = 0.0;

	for (int i = 0; i < in.size(); i++)
	{
		s += pow(eDist(in[i].a->o, in[i].b->o),2);
	}

	return 0.5 * s;
}

double computePb(vector<rib> in)
{
	double s = 0.0;

	for (int i = 0; i < in.size(); i++)
	{
		for (int j = 0; j < in[i].a->o.size(); j++)
		{
			double d = in[i].a->o[j] - 2 * in[i].b->o[j] + in[i].c->o[j];
			s += d * d;
		}
	}

	return 0.5 * s;
}

void ElasticMap::train(vector<dat> data)
{
	double lr = 0.00001;
	double bendingCoefficient = 1.0;
	double stretchCoefficient = 0.5;
	vector<datToNode> sSet;
	for (int i = 0; i < data.size(); i++)
	{
		datToNode d;
		d.d.s = data[i].s;
		d.d.t = data[i].t;
		d.d.sVec = data[i].sVec;
		d.d.tVec = data[i].tVec;
		d.n = getCNode(data[i].sVec);

		sSet.push_back(d);
	}

	cout << "sS " << sSet.size() << endl; 

	uniform_int_distribution<int> uin(0, nodes.size() - 1);
	int numRibs = (uin(rng) + 1)/ 3;

	vector<rib> bSet;
	for (int i = 0; i < numRibs; i++)
	{
		rib r;
		r.a = &nodes[uin(rng)];
		r.b = &nodes[uin(rng)];
		r.c = &nodes[uin(rng)];

		bSet.push_back(r);
	}

	cout << "bS " << bSet.size() << endl;

	//Todo: make lattice instead of random.
	vector<band> eSet;
	for (int i = 0; i < nodes.size(); i++)
	{
		band b;
		b.a = &nodes[i];
		b.b = &nodes[uin(rng)];

		eSet.push_back(b);
	}

	cout << "eS " << eSet.size() << endl;


	cout << "Ps: " << computePs(sSet) << endl;
	cout << "Pe: " << stretchCoefficient * computePe(eSet) << endl;
	cout << "Pb: " << bendingCoefficient * computePb(bSet) << endl;
	double delta = -1.0;
	unsigned long it = 0;
	while (it < 1000 && (delta > 1.0 || delta < 0.0))
	{
		delta = 0.0;

		cout << "Resetting error." << endl;
		for (int i = 0; i < nodes.size(); i++)
		{
			for (int j = 0; j < nodes[i].e.size(); j++)
			{
				nodes[i].e[j] = 0.0;
			}
		}

		cout << "Calculating stretching energy gradients." << endl;
		for (int i = 0; i < sSet.size(); i++)
		{
			for (int j = 0; j < sSet[i].n->e.size(); j++)
			{
				sSet[i].n->e[j] += sSet[i].d.tVec[j] - sSet[i].n->o[j];
			}
		}

		cout << "Calculating elastic energy gradients." << endl;
		for (int i = 0; i < eSet.size(); i++)
		{
			for (int j = 0; j < eSet[i].a->e.size(); j++)
			{
				eSet[i].a->e[j] += stretchCoefficient * (eSet[i].b->e[j] - eSet[i].a->e[j]);
				eSet[i].b->e[j] += stretchCoefficient * (eSet[i].a->e[j] - eSet[i].b->e[j]);
			}
		}

		cout << "Calculating bending energy gradients." << endl;
		for (int i = 0; i < bSet.size(); i++)
		{
			for (int j = 0; j < bSet[i].a->e.size(); j++)
			{
				bSet[i].a->e[j] += bendingCoefficient * (2.0 * bSet[i].b->o[j] - bSet[i].a->o[j] - bSet[i].c->o[j]);
				bSet[i].b->e[j] += -1.0 * bendingCoefficient * (2.0 * bSet[i].b->o[j] - bSet[i].a->o[j] - bSet[i].c->o[j]);
				bSet[i].c->e[j] += bendingCoefficient * (2.0 * bSet[i].b->o[j] - bSet[i].a->o[j] - bSet[i].c->o[j]);
			}
		}

		cout << "Applying gradients." << endl;
		for (int i = 0; i < nodes.size(); i++)
		{
			for (int j = 0; j < nodes[i].o.size(); j++)
			{
				delta += lr * fabs(nodes[i].e[j]);
				nodes[i].o[j] += lr * nodes[i].e[j];
			}
		}

		it++;

		double Ps = computePs(sSet);
		double Pe = stretchCoefficient * computePe(eSet);
		double Pb = bendingCoefficient * computePb(bSet);
		cout << "\n[" << it << "] -- delta: " << delta << " / 1.0"<< endl;
		cout << "\tP: " << Ps + Pe + Pb << endl;
		cout << "\tPs: " << Ps << endl;
		cout << "\tPe: " << Pe << endl;
		cout << "\tPb: " << Pb << endl;
	}

	
}

vector<oDat> ElasticMap::output(vector<iDat> in)
{
	vector<oDat> out;

	for (int i = 0; i < in.size(); i++)
	{
		cout << "Generating Output: " << (double)i / in.size() << "\r";
		if (i % 5 == 0)
			cout.flush();
		vector<double> tempO = getOut(in[i].iVec);
		oDat d;
		d.s = in[i].s;
		d.t = in[i].t;
		d.oVec = tempO;
		out.push_back(d);
	}
	cout << "Generating Output: 1            " << endl;

	return out;
}

