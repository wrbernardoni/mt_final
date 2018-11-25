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

vector<dat> loadTraining(string);
vector<oDat> loadEval(string);
void outputEval(vector<oDat>, string);

#endif

