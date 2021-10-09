#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "Sample.h"
#include "Configure.h"
#include <cmath>
#include "ps/ps.h"

namespace LR
{
    void sigmoid(float &x);

    /*
 * @TODO add multithreading || 
 *       print some information when program runing ||
 *       overload predict() which can reture the result
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
        void calculateLoss(std::vector<float> &weight, std::vector<Data> &batchdata, std::vector<float> &loss);

        /*
     * @param loss the value of loss function
     * @param gradient the gradient used to push
     * @param alpha the learing rate
     */
        void calculateGradient(std::vector<float> &loss, std::vector<Data> &batchdata, std::vector<float> &gradient, const float &alpha);

        /*
     * @brief save the parameter of model to the file named "Model.txt"
     */
        void saveModel(Configure &config);

        /*
     * @param config the configure file from LR
     */
        void train(Configure &config, Sample &sample);

        /*
     * @brief when preciting, call this function
     * @param hvalue store the value fo hypothesis
     */
        void calculateHypothesis(std::vector<float> &weight, std::vector<Data> &batchdata, std::vector<float> &hvalue);

        void predict(Configure &config);

        friend void sigmoid(float &x);

    private:
        ps::KVWorker<float> *_kv;
    };

}

#endif