#include "Configure.h"

Configure::Configure(char* config_file)
{
	cout << "start to read configure file" << endl;
	ifstream config;
	config.open(config_file, ios::in);
	config >> _feature_num >> _input_size >> _train_epoch >> _minibatch_size >> _alpha >> _train_file >> _test_file;
	config.close();
}