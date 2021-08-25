#ifndef SAMPLE_H
#define SAMPLE_H
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;


/*
 * @brief store one sample, the first feature = 1
 */
typedef struct Data
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
     * @param rowNum the number of data
     * @param colNum equals feature_num - 1
     */
    Sample(const char* dataPath = NULL, int rowNum = 0, int colNum = 0, int batchsize = 10);


    

    /*
     * @return feature num
     */ 
    int getFeatureNum();

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