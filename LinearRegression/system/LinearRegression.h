#ifndef LINEAR_REGRESSION_H
#define LINEAR_REGRESSION_H
#include "Configure.h"
#include "Model.h"
#include "Sample.h"
#include "ps/ps.h"

class LinearRegression
{
public:

    /*
     * @param config_file the file path of configure file
     */
    LinearRegression(char *config_file);

    void train();
    void saveModel();
    //void loadModel();


    void predict();

private:
    Configure _config;
    Model _model;
};




#endif
