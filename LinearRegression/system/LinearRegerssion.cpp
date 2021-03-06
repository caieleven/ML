#include "LinearRegression.h"

LinearRegression::LinearRegression(char *config_file):_config(config_file)
{

}

void LinearRegression::train()
{
    Sample sample(_config._train_file.c_str(), _config._input_size, _config._feature_num, _config._minibatch_size);
    for(int i = 0; i < _config._train_epoch; i++)
    {
        _model.train(_config, sample);
    }
}

void LinearRegression::saveModel()
{
    _model.saveModel(_config);
}


void LinearRegression::predict()
{
    if(ps::MyRank() == 0)
        _model.predict(_config);
}

