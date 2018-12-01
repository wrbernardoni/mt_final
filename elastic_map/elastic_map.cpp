#include "loadTraining.h"
#include "map_class.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
  if (argc < 3)
  {
    cout << "Too few arguments, needs training, test and output file \n";
    return 1;
  }

  vector<dat> data = loadTraining(argv[1]);
  if (data.size() == 0)
  {
    cout << "No data read.\n";
    return 1;
  }

  ElasticMap map;

  map.train(data);

  vector<dat> test = loadTraining(argv[2]);
  
  vector<oDat> out = map.output(test);
  outputEval(out, argv[3]);
  return 0;
}


