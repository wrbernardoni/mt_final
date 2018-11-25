#include "loadTraining.h"
#include <iostream>
#include <cmath>
#include <map>

using namespace std;

int main(int argc, char** argv)
{
  if (argc < 3)
  {
    cout << "Too few arguments, needs Output File and Test file\n";
    return 1;
  }
  vector<oDat> out = loadEval(argv[1]);
  vector<dat> test = loadTraining(argv[2]);

  map<string, vector<double>> oVecs;
  for (int i = 0; i < out.size(); i++)
  {
    oVecs[out[i].s] = out[i].oVec;
  }


  long double totalSquaredError = 0.0;
  unsigned int count = 0;
  for (int i = 0; i < test.size() - 1; i++)
  {
    if (oVecs.count(test[i].s) == 0)
    {
      cout << "Missing output vector for " << test[i].s << endl;
      continue;
    }

    count++;
    long double se = 0.0;
    for (int j = 0; j < test[i].tVec.size(); j++)
    {
      se += pow(test[i].tVec[j] - oVecs[test[i].s][j], 2);
    }
    cout << test[i].s << "->" << test[i].t << "--SE: " << se << endl;
    totalSquaredError += se;
  }
  cout << endl << endl;
  cout << "TOTAL SQUARED ERROR: " << totalSquaredError << endl;
  cout << "MEAN SQUARED ERRPR: " << totalSquaredError / count << endl;
  return 1;
}

