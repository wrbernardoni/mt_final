#include "loadTraining.h"
#include "map_class.h"
#include <iostream>

using namespace std;

#define DIMENSIONS 100
#define NUM_NODES 10000

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
  double min = 0.0;
  double max = 0.0;
  for (int i = 0; i < data.size(); i++)
  {
  	for (int j = 0; j < data[i].sVec.size(); j++)
  	{
  		if (data[i].sVec[j] < min)
  			min = data[i].sVec[j];

  		if (data[i].sVec[j] > max)
  			max = data[i].sVec[j];
  	}

  	for (int j = 0; j < data[i].sVec.size(); j++)
  	{
  		if (data[i].tVec[j] < min)
  			min = data[i].tVec[j];

  		if (data[i].tVec[j] > max)
  			max = data[i].tVec[j];
  	}
  }

  ElasticMap map(DIMENSIONS, NUM_NODES, min, max);

  cout << "Training elastic map.\n";
  cout.flush();
  map.train(data);

  cout << "Loading test data.\n";
  cout.flush();
  vector<iDat> test = loadTest(argv[2]);

  cout << "Generating outputs.\n";
  cout.flush();
  vector<oDat> out = map.output(test);
  outputEval(out, argv[3]);
  return 0;
}


