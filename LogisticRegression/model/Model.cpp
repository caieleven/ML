#include "Model.h"

/*
 * @brief change x directly
 */
void sigmoid(float& x)
{
    double ex = pow(2.718281828, x);
    x = ex / (1.0 + ex);
}



Model::Model()
{
    _kv = new ps::KVWorker<float>(0);
}


Model::~Model()
{
    delete _kv;
}

void Model::train(Configure& config, Sample& sample)
{
    //Sample sample(config._train_file, config._input_size, config._feature_num, config._minibatch_size);
    vector<ps::Key> keys(sample.getFeatureNum());
    vector<float> weight(sample.getFeatureNum());
    vector<float> gradient(sample.getFeatureNum());
    vector<float> loss(config._minibatch_size);
    for(int i = 0; i < sample.getFeatureNum(); i++)
        keys[i] = i;

    while(sample.loadNextMinibatchSample())
    {
        _kv->Wait(_kv->Pull(keys, &weight));
        calculateLoss(weight, sample.batch_data, loss);
        calculateGradient(loss,sample.batch_data, gradient, config._alpha);
        _kv->Wait(_kv->Push(keys, gradient));
    }
}

void Model::calculateLoss(vector<float>& weight, vector<Data>& batchdata, vector<float>& loss)
{
    float tempValue;
    loss.clear();
    for(int i = 0; i < batchdata.size(); i++)
    {
        tempValue = 0;
        for(int j = 0; j < weight.size(); j++)
        {
            tempValue += weight[j] * batchdata[i].features[j];
        }
        sigmoid(tempValue);
        tempValue -= batchdata[i].label;
        loss.push_back(tempValue);
    }
}


void Model::calculateGradient(vector<float>& loss, vector<Data>& batchdata, vector<float>& gradient, const float& alpha)
{
    float meanLoss = 0;
    for(int i = 0; i < loss.size(); i++)
        meanLoss += loss[i];
    meanLoss /= loss.size();
    for(int i = 0; i < gradient.size(); i++)
    {
        meanLoss = 0;
        for(int j = 0; j < loss.size(); j++)
        {
            meanLoss += loss[j] * batchdata[j].features[i];
        }
        meanLoss /= loss.size();
        gradient[i] = alpha * meanLoss;
    }
}


void Model::saveModel(Configure& config)
{
    vector<ps::Key> keys(config._feature_num + 1);
    vector<float> weight(config._feature_num + 1);
    for(int i = 0; i< keys.size(); i++)
        keys[i] = i;
    _kv->Wait(_kv->Pull(keys, &weight));
    ofstream output("Model.txt");
    if(output.is_open())
    {
        for(int i = 0; i < weight.size(); i++)
        {
            output << weight[i] << "\t";
        }
        output.close();
    }
}



void Model::predict(Configure& config)
{
    vector<ps::Key> keys(config._feature_num + 1);
    vector<float> weight(config._feature_num + 1);
    for(int i = 0; i< keys.size(); i++)
        keys[i] = i;
    _kv->Wait(_kv->Pull(keys, &weight));

    int testSize = -1; // means all test data
    int testBatchSize = 10;
    float acc = 0;
    Sample testData(config._test_file.c_str(), testSize, config._feature_num, testBatchSize);
    vector<float> hvalue(testData.getFeatureNum());
    vector<float> testResult(testSize);
    while(testData.loadNextMinibatchSample())
    {
        calculateHypothesis(weight, testData.batch_data, hvalue);
        for(int i = 0; i < hvalue.size(); i++)
        {
            if(hvalue[i] >= 0.5)
            {
                testResult.push_back(1);
                if(testData.batch_data[i].label == 1)
                    acc += 1;
            }
                
            else
                testResult.push_back(0);
        }
    }
    acc /= testSize;
}



void Model::calculateHypothesis(vector<float>& weight, vector<Data>& batchdata, vector<float>& hvalue)
{
    hvalue.clear();
    float tempv = 0;
    for(int i = 0; i < batchdata.size(); i++)
    {
        tempv = 0;
        for(int j = 0; j < weight.size(); j++)
        {
            tempv += weight[j] * batchdata[i].features[j];
        }
        sigmoid(tempv);
        hvalue.push_back(tempv);
    }

}