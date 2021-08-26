#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "Sample.h"
#include "Configure.h"
#include <cmath>
#include "ps/ps.h"
using namespace std;



void sigmoid(float& x);

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
    void train(Configure& config, Sample& sample);

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


#endif