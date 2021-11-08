#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <string>
#include <fstream>
#include <iostream>
#include "log.h"
namespace LR
{
    struct Configure
    {
        Configure(char *config_file);
        Configure();

        int _feature_num;
        int _input_size;
        //int _output_size;
        int _train_epoch;
        int _minibatch_size;
        float _alpha;
        std::string _train_file;
        std::string _test_file;
    };
}

#endif
