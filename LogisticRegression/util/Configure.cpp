#include "Configure.h"

namespace LR
{
	Configure::Configure(char *config_file)
	{
		std::cout << "start to read configure file" << std::endl;
		std::ifstream config;
		config.open(config_file, std::ios::in);
		config >> _feature_num >> _input_size >> _train_epoch >> _minibatch_size >> _alpha >> _train_file >> _test_file;
		config.close();
	}

}