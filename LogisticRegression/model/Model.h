#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "Sample.h"
#include "Configure.h"
#include <cmath>
#include "ps/ps.h"
using namespace std;


float sigmoid();


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
    void calculateGradient(vector<float> loss, vector<float> gradient, float alpha);


    void saveModel(Configure& config);

    /*
     * @param config the configure file from LR
     */
    void train(Configure& config);

private:
    ps::KVWorker<float>* _kv;
}

Model::Model()
{
    _kv = new ps::KVWorker<float>(0);
}


Model::train(Configure& config)
{
    Sample sample(config._train_file, config._input_size, config._feature_num, config._minibatch_size);
    vector<ps::Key> keys(sample.getFeatureNum());
    vector<float> weight;
    vector<float> gradient;
    vector<float> loss(config._minibatch_size);
    for(int i = 0; i < sample.getFeatureNum(); i++)
        keys[i] = i;

    while(sample.loadNextMinibatchSample)
    {
        _kv->Wait(_kv->Pull(keys, &weight));
        calculateLoss(weight, sample.batch_data, loss);
        calculateGradient(loss, gradient, config._alpha);
        _kv->Wait(_kv->Push(keys, gradient));
    }
}

void Model::calculateLoss(vector<float>& weight, vector<Data>& batchdata, vector<float>& loss)
{

}


void Model::calculateGradient(vector<float> loss, vector<float> gradient, float alpha)
{
    
}




#endif