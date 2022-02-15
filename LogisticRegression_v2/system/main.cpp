#include "logreg.h"


int main()
{
    std::string config("/home/caieleven/ML/LogisticRegression_v2/ScriptsAndDate/config.txt");
    LR::LogReg<float> logreg(config);
    logreg.Train();
    logreg.SaveModel();
    return 0;
}