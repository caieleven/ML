#ifndef LOGISTIC_REGRESSION_H
#define LOGISTIC_REGRESSION_H
#include "util/Configure.h"
#include "model/Model.h"
#include "util/Sample.h"

class LogisticRegression
{
public:

    /*
     * @param config_file the file path of configure file
     */
    LogisticRegression(char *config_file);
    ~LogisticRegression();

    void train();
    void saveModel();
    void loadModel();


    void predict();

private:
    Configure _config;
    Model _model;
};




#endif
