#include "configure.h"
#include <unordered_map>
#include "io.h"
#include <string>

#define VALUEMATCH(key, type)   \
    ValueMatch(map, type, #key, &key)


namespace LR
{
    Configure::Configure(const std::string &config_file)
    {
        TextReader reader(config_file);
        std::unordered_map<std::string, std::string> map;
        map.reserve(15);
        std::string line;
        //read all of the parameters and store them in the map
        //in the form of key-value pairs
        while (reader.GetLine(line))
        {
            int pos = line.find("=");
            map[line.substr(0, pos)] = line.substr(pos + 1, line.length() - pos - 1);
        }
        VALUEMATCH(train_file, "string");
        VALUEMATCH(test_file, "string");
        VALUEMATCH(regularizer, "string");
        VALUEMATCH(objective_type, "string");
        VALUEMATCH(optimizer, "string");
        VALUEMATCH(output_file, "string");
        VALUEMATCH(model_file, "string");
        VALUEMATCH(input_dimention, "int");
        VALUEMATCH(output_dimention, "int");
        VALUEMATCH(read_buffer_size, "int");
        VALUEMATCH(mini_batch_size, "int");
        VALUEMATCH(train_epoch, "int");
        VALUEMATCH(learning_rate, "float");
        VALUEMATCH(regular_coef, "float");
        VALUEMATCH(use_ps, "bool");


    }

    void Configure::ValueMatch(std::unordered_map<std::string, std::string> &map, std::string datatype, std::string key, void *keyp)
    {
        if(map.find(key) == map.end())
            return;
        if(datatype == "int")
        {
            *(static_cast<int*>(keyp)) = atoi(map[key].c_str());
        }
        else if(datatype == "float")
        {
            *(static_cast<float*>(keyp)) = atof(map[key].c_str());
        }
        else if(datatype == "string")
        {
            *(std::string*)keyp = map[key];
        }
        else if(datatype == "bool")
        {
            *(static_cast<bool*>(keyp)) = map[key] == "true" ? true:false;
        }
        else if(datatype == "double")
        {
            *(static_cast<double*>(keyp)) = strtod(map[key].c_str(), nullptr);
        }
        else
        {
            //log error
        }
    }
}