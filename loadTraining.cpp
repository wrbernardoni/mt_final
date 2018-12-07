#include "loadTraining.h"

#include <fstream>

using namespace std;

vector<dat> loadTraining(string fn)
{
  vector<dat> data;
  ifstream in(fn);
  if (!in.is_open())
    return data;

  while (!in.eof())
  {
    dat d;
    in >> d.s;
    in >> d.t;
    for (int i = 0; i < 100; i++)
    {
      double t;
      in >> t;
      d.sVec.push_back(t);
    }
    string l;
    in >> l;
    for (int i = 0; i < 100; i++)
    {
      double t;
      in >> t;
      d.tVec.push_back(t);
    }

    data.push_back(d);
  }

  return data;
}


vector<oDat> loadEval(string fn)
{
  vector<oDat> data;
  ifstream in(fn);
  if (!in.is_open())
    return data;

  while (!in.eof())
  {
    oDat d;
    in >> d.s;
    in >> d.t;
    for (int i = 0; i < 100; i++)
    {
      double t;
      in >> t;
      d.oVec.push_back(t);
    }

    data.push_back(d);
  }

  return data;
}

vector<iDat> loadTest(string fn)
{
  vector<iDat> data;
  vector<dat> all = loadTraining(fn);
  for (int i = 0; i < all.size(); i++)
  {
    iDat t;
    t.s = all[i].s;
    t.t = all[i].t;
    t.iVec = all[i].sVec;
    data.push_back(t);
  }

  return data;
}

void outputEval(vector<oDat> data, string fn)
{
  ofstream out(fn);

  for (int i = 0; i < data.size(); i++)
  {
    out << data[i].s << " ";
    out << data[i].t;
    for (int j = 0; j < data[i].oVec.size(); j++)
    {
      out << " " << data[i].oVec[j];
    }
    out << endl;
  }
}
