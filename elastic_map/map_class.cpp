#include "map_class.h"
#include <iostream>
#include <cmath>
#include <list>

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

EM_Node* ElasticMap::getTCNode(vector<double> in)
{
	double minDist = -1.0;
	EM_Node* node = NULL;

	minDist = -1.0;
	for (int i = 0; i < nodes.size(); i++)
	{
		double d = eDist(in, nodes[i].i);
		if (d < minDist || minDist < 0.0)
		{
			minDist = d;
			node = &nodes[i];
		}
	}

	return node;
}

//TODO: Interpolate and optimize here
vector<double> ElasticMap::getOut(vector<double> in)
{
	vector<double> dist;
	double norm = 0.0;
	vector<double> interp;
	const double P = 5.0;

	for (int i = 0; i < nodes.size(); i++)
	{
		double dT = eDist(nodes[i].i, in);
		dist.push_back(pow(dT, -P));

		if (dT == 0)
		{
			return nodes[i].o;
		}

		norm += dist[i];
	}

	for (int i = 0; i < nodes[0].i.size(); i++)
	{
		interp.push_back(0.0);
	}
	

	for (int j = 0; j < nodes.size(); j++)
	{
		for (int k = 0; k < nodes[j].o.size(); k++)
		{
			interp[k] += nodes[j].o[k] * dist[j];
		}
	}

	for (int i = 0; i < interp.size(); i++)
	{
		interp[i] = interp[i] / norm;
	}

	return interp;//getTCNode(in)->o;
}

ElasticMap::ElasticMap(unsigned int dim, unsigned int nn, double sdev, vector<dat> data)
{
	normal_distribution<double> unif(0, sdev);

	cout << "Generating " << nn << " nodes" << endl;
	for(int i = 0; i < data.size(); i++)
	{
		EM_Node newN;
		for (int j = 0; j < dim; j++)
		{
			newN.i.push_back(data[i].sVec[j]);
			newN.o.push_back(data[i].sVec[j]);
			newN.e.push_back(0);
		}
		nodes.push_back(newN);
	}
	for (int i = nodes.size(); i < nn; i++)
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

	cout << "Creating " << (int)(nodes.size()/30)+1 << " node clusters" << endl;
	uniform_int_distribution<int> choice(0, nodes.size());
	for(int i = 0; i < (nodes.size()/30)+1; i++)//(pow(10.0, (log10(nodes.size()) / 1.5)) + 1); i++)
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
		cout << "\tAllocating Clusters: " << (double)i / nodes.size() << "            \r";
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
		cout << "\tRecomputing Centroids: " << (double)i / k_means.size() << "            \r";
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

vector<band> constructLattice(vector<EM_Node>* nodes, vector<Cluster>* kmns)
{
	vector<band> lattice;

	list<Cluster*> added;
	list<Cluster*> remaining;
	added.push_back(&((*kmns)[0]));
	for (int k = 1; k < kmns->size(); k++)
	{
		remaining.push_back(&(*kmns)[k]);
	}

	while (remaining.size() > 0)
	{
		cout << "\tCluster MST: " << remaining.size() << " remaining.      \r";
		if (remaining.size() % 10 == 0)
			cout.flush();

		double mind = -1.0;
		Cluster* n = NULL;
		Cluster* s = NULL;
		for (list<Cluster*>::iterator it=remaining.begin(); it != remaining.end(); it++)
		{
			if (n == NULL)
				n = *it;

			for (list<Cluster*>::iterator it2=added.begin(); it2 != added.end(); it2++)
			{
				double d = eDist((*it)->centroid, (*it2)->centroid);
				if (((d < mind || mind < 0) && (!(*it)->dead)) || n->dead)
				{
					n = *it;
					s = *it2;
				}
			}
		}

		remaining.remove(n);

		if (n->dead || s->dead)
			continue;

		int i_k = 0;
		int i_l = 0;
		double i_k_l = -1.0;

		for (int i = 0; i < n->nodes.size(); i++)
		{
			double mind = -1.0;
			int minj = -1;
			for (int j = 0; j < s->nodes.size(); j++)
			{
				double d = eDist(n->nodes[i]->i, s->nodes[j]->i);
				if (d < mind || minj == -1)
				{
					mind = d;
					minj = j;
				}
			}

			if (mind < i_k_l || i_k_l < 0.0)
			{
				i_k_l = mind;
				i_k = i;
				i_l = minj;
			}
		}

		band b;
		b.a = n->nodes[i_k];
		b.b = s->nodes[i_l];
		lattice.push_back(b);
	}

	cout << "\tCluster MST: 0 remaining.      " << endl;

	for (int k = 0; k < kmns->size(); k++)
	{
		cout << "\tCluster Lattices: " << (double)k / kmns->size() << "                     \r";
		if (k % 15 == 0)
			cout.flush();

		if ((*kmns)[k].dead)
			continue;

		for (int i = 0; i < (*kmns)[k].nodes.size(); i++)
		{
			for (int j = i + 1; j < (*kmns)[k].nodes.size(); j++)
			{
				band b;
				b.a = (*kmns)[k].nodes[i];
				b.b = (*kmns)[k].nodes[j];

				lattice.push_back(b);
			}
		}
	}

	cout << "\tCluster Lattices: 1                  " << endl;

	return lattice;
}

void ElasticMap::train(vector<dat> data)
{
	unsigned long maxIT = 10000;
	double lr = 0.001;
	double bendingCoefficient = 1.0;
	double stretchCoefficient = 1.0;
	double sr = 1.0;
	double br = 1.0;
	vector<datToNode> sSet;
	for (int i = 0; i < data.size(); i++)
	{
		datToNode d;
		d.d.s = data[i].s;
		d.d.t = data[i].t;
		d.d.sVec = data[i].sVec;
		d.d.tVec = data[i].tVec;
		d.n = getTCNode(data[i].sVec);

		sSet.push_back(d);
	}

	cout << "sS " << sSet.size() << endl; 

	uniform_int_distribution<int> uin(0, nodes.size() - 1);
	int numRibs = (nodes.size())/ 30;

	vector<rib> bSet;
	for (int i = 0; i < numRibs; i++)
	{
		rib r;
		int a = uin(rng);
		int b = uin(rng);

		r.a = &nodes[a];
		r.c = &nodes[b];

		vector<double> t;
		for (int i = 0; i < r.a->i.size(); i++)
		{
			double tmp = (r.a->i[i] + r.c->i[i])/2.0;
		}

		r.b = getCNode(t);
		

		bSet.push_back(r);
	}

	cout << "bS " << bSet.size() << endl;

	vector<band> eSet = constructLattice(&nodes, &k_means);

	cout << "eS " << eSet.size() << endl;

	br = 1.0;//(double)sSet.size() / (4.0 * (double)bSet.size());
	sr = 1.0;//(double)sSet.size() / (2.0 * (double)eSet.size());


	cout << "Ps: " << computePs(sSet) << endl;
	cout << "Pe: " << stretchCoefficient * computePe(eSet) << endl;
	cout << "Pb: " << bendingCoefficient * computePb(bSet) << endl;
	double delta = -1.0;
	unsigned long it = 0;
	while (it < maxIT)// && (delta > 1.0 || delta < 0.0))
	{
		bendingCoefficient = br * (1.0 - sqrt((double)it / maxIT));
		stretchCoefficient = sr * (1.0 - sqrt((double)it / maxIT));
		delta = 0.0;

		cout << "Resetting error." << endl;
		for (int i = 0; i < nodes.size(); i++)
		{
			for (int j = 0; j < nodes[i].e.size(); j++)
			{
				nodes[i].e[j] = 0.0;
			}
		}

		cout << "Calculating elastic energy gradients." << endl;
		for (int i = 0; i < eSet.size(); i++)
		{
			for (int j = 0; j < eSet[i].a->e.size(); j++)
			{
				eSet[i].a->e[j] += stretchCoefficient * (eSet[i].b->o[j] - eSet[i].a->o[j]);
				eSet[i].b->e[j] += stretchCoefficient * (eSet[i].a->o[j] - eSet[i].b->o[j]);
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

		cout << "Calculating stretching energy gradients." << endl;
		for (int i = 0; i < sSet.size(); i++)
		{
			for (int j = 0; j < sSet[i].n->e.size(); j++)
			{
				sSet[i].n->e[j] = sSet[i].d.tVec[j] - sSet[i].n->o[j];
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
		cout << "\n[" << it << " | " << maxIT << "] -- delta: " << delta << endl;
		cout << "sc: " << stretchCoefficient << " bc: " << bendingCoefficient << endl;
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

