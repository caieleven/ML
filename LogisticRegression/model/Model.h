#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "Sample.h"
#include "Configure.h"
#include <cmath>
#include "ps/ps.h"
using namespace std;



/*
 * @brief change x directly
 */
void sigmoid(float& x)
{
    double ex = pow(2.718281828, x);
    x = ex / (1.0 + ex);
}


/*
template<class ElemType>
ElemType sigmoid(ElemType x);

template<class ElemType>
class Model
{
private:
    int _num;
    vector<ElemType> *_model;
    ps::KVWorker<ElemType> * _kv;

public:
    Model(int num = 1);
    ~Model();
    static Model<ElemType>* getModel(int size);

    void pullModel(vector<ElemType> &model);
    void pushModel(vector<ElemType> &model);
    double getGradient(vector<ElemType> &rowSample);
//    void updata(Sample<ElemType> *sample);

    friend ElemType sigmoid<ElemType>(ElemType x);
};


template <class ElemType>
Model<ElemType>::Model(int num) : _num(num)
{
    _model = new vector<ElemType>(_num, 1);
}

template <class ElemType>
Model<ElemType>::~Model()
{
    delete[] _model;
    delete _kv;
}

template <class ElemType>
Model<ElemType> *Model<ElemType>::getModel(int size)
{
    return new Model(size);
}

template <class ElemType>
void Model<ElemType>::pullModel(vector<ElemType> &model)
{
    model.assign(_model->begin(), _model->end());
}

template <class ElemType>
void Model<ElemType>::pushModel(vector<ElemType> &model)
{
    _model->assign(model.begin(), model.end());
}

template <class ElemType>
double Model<ElemType>::getGradient(vector<ElemType>& rowSample)
{
    double gradient = 0;
    int i = 0;
    for (i = 0; i < _num; i++)
    {
        gradient += (*_model)[i] * rowSample[i];
    }
    gradient = rowSample[i] - sigmoid(gradient);
    return gradient;
}

template <class ElemType>
ElemType sigmoid(ElemType x)
{
    return 1 / (1 + exp(-x));
}
*/

class Model
{
public:
    Model();
    ~Model();

    /*
     * @brief to calculate the value of loss function
     * @param weight the theta
     * @param batchdata the traing data
     * @param loss the value fo loss function
     */
    void calculateLoss(vector<float>& weight, vector<Data>& batchdata, vector<float>& loss);

    /*
     * @param loss the value of loss function
     * @param gradient the gradient used to push
     * @param alpha the learing rate
     */
    void calculateGradient(vector<float>& loss, vector<Data>& batchdata, vector<float>& gradient, const float& alpha);

    /*
     * @brief save the parameter of model to the file named "Model.txt"
     */
    void saveModel(Configure& config);

    /*
     * @param config the configure file from LR
     */
    void train(Configure& config, Sample& sample);\

    /*
     * @brief when preciting, call this function
     * @param hvalue store the value fo hypothesis
     */
    void calculateHypothesis(vector<float>& weight, vector<Data>& batchdata, vector<float>& hvalue);

    void predict(Configure& config);

    friend void sigmoid(float& x);

private:
    ps::KVWorker<float>* _kv;
};

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

    int testSize = 10;
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

#endif