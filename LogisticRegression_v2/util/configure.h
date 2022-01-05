#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <string>
#include <unordered_map>
#include "log.h"

namespace LR
{
    class Configure
    {
    public:
        Configure(const std::string &config_file);

        std::string train_file;
        std::string test_file;
        std::string regular_type = "";
        std::string objective_type = "sigmoid";
        std::string output_file = "LR.output";
        std::string model_file = "LR.model";

        int input_dimention;
        int output_dimention;
        int read_buffer_size = 1024;
        //int input_size;
        int mini_batch_size = 20;
        int train_epoch = 1;
        float learning_rate = 0.5;
        bool use_ps = false;
    private:
        void ValueMatch(std::unordered_map<std::string, std::string> &map, std::string datatype, std::string key, void *keyp);
    };
}

#endif