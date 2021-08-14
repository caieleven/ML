#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <string>
#include <fstream>
using namespace std;


struct Configure
{
	Configure(char* config_file);
    Configure();

    int _feature_num;
    int _input_size;
    //int _output_size;
    int _train_epoch;
	int _minibatch_size;
    float _alpha;
	string _train_file;
	string _test_file;
    

};

#endif 
