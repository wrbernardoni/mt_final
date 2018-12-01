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

  cout << "Loading training data.\n";
  cout.flush();
  vector<dat> data = loadTraining(argv[1]);
  if (data.size() == 0)
  {
    cout << "No data read.\n";
    return 1;
  }

  cout << "Generating elastic map.\n";
  cout.flush();
  ElasticMap map;

  cout << "Training elastic map.\n";
  cout.flush();
  map.train(data);

  cout << "Loading test data.\n";
  cout.flush();
  vector<dat> test = loadTraining(argv[2]);

  cout << "Generating outputs.\n";
  cout.flush();
  vector<oDat> out = map.output(test);
  outputEval(out, argv[3]);
  return 0;
}


