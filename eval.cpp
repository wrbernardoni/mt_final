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
  long double totCos = 0.0;
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
    long double dot = 0.0;
    long double an = 0.0;
    long double bn = 0.0;

    for (int j = 0; j < test[i].tVec.size(); j++)
    {
      se += pow(test[i].tVec[j] - oVecs[test[i].s][j], 2);
      dot += test[i].tVec[j] * oVecs[test[i].s][j];
      an += test[i].tVec[j] * test[i].tVec[j];
      bn += oVecs[test[i].s][j] * oVecs[test[i].s][j];
    }
    totalSquaredError += se;
    totCos += dot / (sqrt(an) * sqrt(bn));
  }

  long double mse = totalSquaredError / count;
  long double totVa = 0.0;
  long double cosVa = 0.0;

  for (int i = 0; i < test.size() - 1; i++)
  {
    if (oVecs.count(test[i].s) == 0)
    {
      cout << "Missing output vector for " << test[i].s << endl;
      continue;
    }
    long double dot = 0.0;
    long double an = 0.0;
    long double bn = 0.0;
    long double se = 0.0;
    long double va = 0.0;
    for (int j = 0; j < test[i].tVec.size(); j++)
    {
      se += pow(test[i].tVec[j] - oVecs[test[i].s][j], 2);
      dot += test[i].tVec[j] * oVecs[test[i].s][j];
      an += test[i].tVec[j] * test[i].tVec[j];
      bn += oVecs[test[i].s][j] * oVecs[test[i].s][j];
    }

    va = pow(mse - se, 2);
    cosVa += pow(totCos / count - dot / (sqrt(an) * sqrt(bn)), 2);
    

    cout << test[i].s << "->" << test[i].t << "--SE: " << se << " S.DEV: " << sqrt(va) << " COS: " << dot / (sqrt(an) * sqrt(bn)) << endl;
    totVa += va;
  }

  long double mVa = totVa / count;
  mVa = sqrt(mVa);

  cout << endl << endl;
  cout << "TOTAL SQUARED ERROR: " << totalSquaredError << endl;
  cout << "MEAN SQUARED ERROR: " << totalSquaredError / count << endl;
  cout << "STD DEVIATION: " << mVa << endl;
  cout << "MEAN COS SIM: " << totCos / count << endl;
  cout << "COS SIM S.DEV: " << sqrt(cosVa / count) << endl;
  return 1;
}

