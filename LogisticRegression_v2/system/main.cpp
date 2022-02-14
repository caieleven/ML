#include "logreg.h"


int main()
{
    std::string config("config_file");
    LR::LogReg<float> logreg(config);
    return 0;
}