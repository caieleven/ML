#include "LogisticRegression.h"

namespace LR
{
    LogisticRegression::LogisticRegression(char *config_file) : _config(config_file)
    {
    }

    void LogisticRegression::train()
    {
        Sample sample(_config._train_file.c_str(), _config._input_size, _config._feature_num, _config._minibatch_size);
        Log::Write(LogLevel::Debug, "Start To Train!\n");
        for (int i = 0; i < _config._train_epoch; i++)
        {
            Log::Write(LogLevel::Debug, "Train for %d epoch\n", i + 1);
            _model.train(_config, sample);
        }
    }

    void LogisticRegression::saveModel()
    {
        _model.saveModel(_config);
    }

    void LogisticRegression::predict()
    {
        if (ps::MyRank() == 0)
            _model.predict(_config);
    }

}
