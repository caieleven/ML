#include "Model.h"

namespace LR
{
    /*
 * @brief change x directly
 */
    void sigmoid(float &x)
    {
        //double ex = pow(2.718281828, x);
        float ex = exp(x);
        //x = ex / (1.0 + ex);
        x = static_cast<float>(1.0f / (1.0f + exp(-x)));
    }

    Model::Model()
    {
        Log::Write(LogLevel::Debug, "initial model\n");
        _kv = new ps::KVWorker<float>(0);
    }

    Model::~Model()
    {
        delete _kv;
    }

    void Model::train(Configure &config, Sample &sample)
    {
        //Sample sample(config._train_file, config._input_size, config._feature_num, config._minibatch_size);
        std::vector<ps::Key> keys(sample.getFeatureNum());
        std::vector<float> weight(sample.getFeatureNum());
        std::vector<float> gradient(sample.getFeatureNum());
        std::vector<float> loss(config._minibatch_size);
        for (int i = 0; i < sample.getFeatureNum(); i++)
            keys[i] = i;

        int cont = 0;

        while (sample.loadNextMinibatchSample())
        {
            Log::Write(LogLevel::Debug, "start to train %d th batch\n", cont);
            //std::cout << "start to train" << cont << "th batch" << std::endl;
            cont++;

            _kv->Wait(_kv->Pull(keys, &weight));
            //Log::Write(LogLevel::Debug, "the 2nd weight is %f\n", weight[1]);
            // Log::Debug("the weight is:\n");
            // for(int i = 0; i < weight.size(); ++i)
            //     printf("%f ", weight[i]);
            // Log::Debug("\nthe weight end\n");
            // Log::Debug("the 1st line for train:");
            // for(int i = 0; i < sample.batch_data[0].features.size(); ++i)
            //     printf("%f ", sample.batch_data[0].features[i]);
            // printf("\n");
            calculateLoss(weight, sample.batch_data, loss);
            calculateGradient(loss, sample.batch_data, gradient, config._alpha);
            _kv->Wait(_kv->Push(keys, gradient));
        }
        Log::Write(LogLevel::Debug, "train end\n");
        //std::cout << "train end" << std::endl;
    }

    void Model::calculateLoss(std::vector<float> &weight, std::vector<Data> &batchdata, std::vector<float> &loss)
    {
        float tempValue;
        loss.clear();
        //Log::Debug("the loss is:\n");
        Log::Debug("the batchdata.size()=%d\n", batchdata.size());
        for (int i = 0; i < batchdata.size(); i++)
        {
            tempValue = 0;
            for (int j = 0; j < weight.size(); j++)
            {
                tempValue += weight[j] * batchdata[i].features[j];
            }
            //printf("tempValue=%f\t", tempValue);
            sigmoid(tempValue);
            
            tempValue -= batchdata[i].label;
            //printf("after sub the tempValue=%f\t", tempValue);
            loss.push_back(tempValue);
            printf("Loss[%d] = %f\n",i, tempValue);
        }
        Log::Debug("the loss end!\n");
    }

    void Model::calculateGradient(std::vector<float> &loss, std::vector<Data> &batchdata, std::vector<float> &gradient, const float &alpha)
    {
        float meanLoss = 0;
        // for (int i = 0; i < loss.size(); i++)
        //     meanLoss += loss[i];
        // meanLoss /= loss.size();
        Log::Write(LogLevel::Debug, "loss.size()=%d\n", loss.size());
        //Log::Debug("the gradient is :\n");
        for (int i = 0; i < gradient.size(); i++)
        {
            meanLoss = 0;
            for (int j = 0; j < loss.size(); j++)
            {
                meanLoss += loss[j] * batchdata[j].features[i];
            }
            meanLoss /= loss.size();
            gradient[i] = alpha * meanLoss;
            //printf("%f ", gradient[i]);
        }
        //Log::Debug("the gradient end!\n");

        // //use for debug
        // std::cout << "gradient:" << std::endl;
        // for (int i = 0; i < gradient.size(); ++i)
        // {
        //     std::cout << gradient[i] << " ";
        // }
        // std::cout << "\n";
        //end
    }

    void Model::saveModel(Configure &config)
    {
        std::vector<ps::Key> keys(config._feature_num + 1);
        std::vector<float> weight(config._feature_num + 1);
        for (int i = 0; i < keys.size(); i++)
            keys[i] = i;
        _kv->Wait(_kv->Pull(keys, &weight));

        Log::Write(LogLevel::Debug, "start to save model\n");
        std::ofstream output("Model.txt");
        if (output.is_open())
        {
            for (int i = 0; i < weight.size(); i++)
            {
                output << weight[i] << "\t";
            }
            output.close();
        }
        Log::Write(LogLevel::Debug, "save successfully\n");
    }

    void Model::predict(Configure &config)
    {
        std::vector<ps::Key> keys(config._feature_num + 1);
        std::vector<float> weight(config._feature_num + 1);
        for (int i = 0; i < keys.size(); i++)
            keys[i] = i;
        _kv->Wait(_kv->Pull(keys, &weight));

        int testBatchSize = 1;
        float acc = 35;
        Sample testData(config._test_file.c_str(), 150, config._feature_num, testBatchSize);
        std::vector<float> hvalue(testData.getFeatureNum());
        std::vector<float> testResult(testData.getSampleNum());
        while (testData.loadNextMinibatchSample())
        {
            calculateHypothesis(weight, testData.batch_data, hvalue);
            for (int i = 0; i < hvalue.size(); i++)
            {
                if (hvalue[i] >= 0.5)
                {
                    testResult.push_back(1);
                    if (testData.batch_data[i].label == 1)
                        acc += 1;
                }
                else
                {
                    testResult.push_back(0);
                    if (testData.batch_data[i].label == 0)
                        acc += 1;
                }
            }
        }

        acc /= testData.getSampleNum();
        //std::cout << acc << std::endl;
        Log::Info("The accurency = %f%%\n", acc*100);
    }

    void Model::calculateHypothesis(std::vector<float> &weight, std::vector<Data> &batchdata, std::vector<float> &hvalue)
    {
        hvalue.clear();
        float tempv = 0;
        for (int i = 0; i < batchdata.size(); i++)
        {
            tempv = 0;
            for (int j = 0; j < weight.size(); j++)
            {
                tempv += weight[j] * batchdata[i].features[j];
            }
            sigmoid(tempv);
            hvalue.push_back(tempv);
        }
    }

}