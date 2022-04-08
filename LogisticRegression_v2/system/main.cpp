#include "logreg.h"
#include "server.h"
#include "log.h"
using namespace LR;
using namespace ps;


int main(int argc, char* argv[])
{
    ps::Start(0);
    startServer();
    if (ps::IsWorker())
    {
        // std::string config("/home/caieleven/ML/LogisticRegression_v2/ScriptsAndDate/config.txt");
        std::string config(argv[1]);
        LR::LogReg<float> logreg(config);
        logreg.Train();
        if(ps::MyRank() == 0)
            logreg.Test();
    }
    ps::Finalize(0, true);
    return 0;
}