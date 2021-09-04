#ifndef SAMPLE_H
#define SAMPLE_H
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <sstream>
using namespace std;

/*
 * @brief store one sample, the first feature = 1
 */
struct Data
{
    int label;
    vector<float> features;
};

/*
 *@brief to load the training data
 */

class Sample
{
private:
    // all fo the training data
    vector<Data> _samples;

    // indicate how much data has been used
    long long int _progress = 0;

public:
    //used for training, can be accessed directly
    vector<Data> batch_data;
    int batch_size;

    /*
     * @brief constructor
     * @param rowNum the number of data,if equals -1, means all data
     * @param colNum equals feature_num - 1
     */
    Sample(const char *dataPath = NULL, int rowNum = -1, int colNum = 0, int batchsize = 10);

    /*
     * @return feature num
     */
    inline int getFeatureNum()
    {
        return _samples[0].features.size();
    }

    /*
     *@return row num of sample
     */
    inline int getSampleNum()
    {
        return _samples.size();
    }

    /*
     * @return if true, success
     */
    bool loadNextMinibatchSample();

    // /*
    //  * @brief indicate if all the data has been used
    //  * @return true:this epoch is end
    //  */
    // bool end();
};

#endif