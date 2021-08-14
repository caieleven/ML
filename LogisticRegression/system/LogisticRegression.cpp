#include "LogisticRegression.h"

LogisticRegression::LogisticRegression(char* config_file) : _config(config_file), _model(_config._feature_num + 1)
{
    _sample = new Sample<float>(_config._train_file.c_str(), _config._input_size, _config._feature_num + 1);
}

void LogisticRegression::train()
{
    int i, j, sampleIndex = 0, tempLabel;
    float alpha = 0.005;
    vector<float> modelVector(_config._feature_num + 1);
    vector<float> rowSample(_config._feature_num + 1);
    _model.pullModel(modelVector);
    for (unsigned int i = 0; i < modelVector.size(); i++)
    {
        sampleIndex = 0;
        for (i = 0; i < _config._train_epoch; i++)
        {
            for (j = 0; j < _config._minibatch_size; j++)
            {
                cout << "训练样本:" << sampleIndex << endl;
                _sample->getSampleByIndex(sampleIndex, rowSample, tempLabel);
                modelVector[i] -= alpha * _model.getGradient(rowSample);
                sampleIndex++;
            }
        }
    }
    for(unsigned int i = 0; i < modelVector.size(); i++)
    {
        cout << modelVector[i] << " ";
    }
    _model.pushModel(modelVector);
}

void LogisticRegression::predict()
{
}

void LogisticRegression::saveModel()
{
    
}

void LogisticRegression::loadModel()
{
}


LogisticRegression::~LogisticRegression()
{
    delete _sample;
}
