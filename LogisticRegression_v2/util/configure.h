#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <string>
#include "util/log.h"

class Configure
{
public:
    Configure(const std::string & config_file);


    std::string train_file;
    std::string test_file;
    std::string penalty;

    int input_dimention;
    int output_dimention;

    int input_size;
    int mini_batch_size;
    int epoch;
    float learning_rate;


    
    bool use_ps;



};




#endif