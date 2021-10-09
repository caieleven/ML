#ifndef LOGISTIC_REGRESSION_H
#define LOGISTIC_REGRESSION_H
#include "Configure.h"
#include "Model.h"
#include "Sample.h"
#include "ps/ps.h"
#include "log.h"
namespace LR
{
    class LogisticRegression
    {
    public:
        /*
     * @param config_file the file path of configure file
     */
        LogisticRegression(char *config_file);

        void train();
        void saveModel();
        //void loadModel();

        void predict();

    private:
        Configure _config;
        Model _model;
    };

}

#endif
