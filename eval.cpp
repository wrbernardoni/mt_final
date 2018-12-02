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
    totalSquaredError += se;
  }

  long double mse = totalSquaredError / count;
  long double totVa = 0.0;

  for (int i = 0; i < test.size() - 1; i++)
  {
    if (oVecs.count(test[i].s) == 0)
    {
      cout << "Missing output vector for " << test[i].s << endl;
      continue;
    }
    long double se = 0.0;
    long double va = 0.0;
    for (int j = 0; j < test[i].tVec.size(); j++)
    {
      se += pow(test[i].tVec[j] - oVecs[test[i].s][j], 2);
    }

    va = pow(mse - se, 2);

    cout << test[i].s << "->" << test[i].t << "--SE: " << se << " S.DEV: " << sqrt(va) << endl;
    totVa += va;
  }

  long double mVa = totVa / count;
  mVa = sqrt(mVa);

  cout << endl << endl;
  cout << "TOTAL SQUARED ERROR: " << totalSquaredError << endl;
  cout << "MEAN SQUARED ERROR: " << totalSquaredError / count << endl;
  cout << "STD DEVIATION: " << mVa << endl;
  return 1;
}

