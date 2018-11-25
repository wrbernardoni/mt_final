#include "loadTraining.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
  if (argc < 3)
  {
    cout << "Too few arguments, needs input and output file \n";
    return 1;
  }

  vector<dat> data = loadTraining(argv[1]);
  if (data.size() == 0)
  {
    cout << "No data read.\n";
    return 1;
  }

  vector<oDat> proj;
  for (int i = 0; i < data.size(); i++)
  {
    oDat o;
    o.s = data[i].s;
    o.t = data[i].t;
    o.oVec = data[i].sVec;
    proj.push_back(o);
  }

  outputEval(proj, argv[2]);
  return 0;
}


