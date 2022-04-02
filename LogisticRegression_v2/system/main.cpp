#include "logreg.h"
#include "server.h"
#include "log.h"
using namespace LR;
using namespace ps;


int main()
{
    // ps::Start(0);
    // startServer();
    // if (ps::IsWorker())
    {
        std::string config("/home/caieleven/ML/LogisticRegression_v2/ScriptsAndDate/config.txt");
        LR::LogReg<float> logreg(config);
        logreg.Train();
        // if(ps::MyRank() == 0)
            logreg.Test();
    }
    // ps::Finalize(0, true);
    return 0;
}