#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "util/Sample.h"
#include <cmath>
using namespace std;


//最后一个参数是一个常数值
//初始参数全为1



template<class ElemType>
ElemType sigmoid(ElemType x);

template<class ElemType>
class Model
{
private:
    int _num;
    vector<ElemType> *_model;

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








#endif