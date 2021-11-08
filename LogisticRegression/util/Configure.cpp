#include "Configure.h"

namespace LR
{
	Configure::Configure(char *config_file)
	{
		Log::Write(LogLevel::Debug, "Start to read configure file: %s\n", config_file);
		std::ifstream config;
		config.open(config_file, std::ios::in);
		config >> _feature_num >> _input_size >> _train_epoch >> _minibatch_size >> _alpha >> _train_file >> _test_file;
		config.close();
		Log::Write(LogLevel::Debug, "configure file read end\n");
	}
}