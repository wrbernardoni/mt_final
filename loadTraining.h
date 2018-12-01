#ifndef LOAD_TRAINING_H_
#define LOAD_TRAINING_H_

#include <vector>
#include <string>

using namespace std;

struct dat
{
  string s;
  string t;
  vector<double> sVec;
  vector<double> tVec;
};

struct oDat
{
  string s;
  string t;
  vector<double> oVec;
};

struct iDat
{
  string s;
  string t;
  vector<double> iVec;
};

vector<dat> loadTraining(string);
vector<oDat> loadEval(string);
vector<iDat> loadTest(string);
void outputEval(vector<oDat>, string);

#endif

