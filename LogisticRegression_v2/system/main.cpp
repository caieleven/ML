#include "logreg.h"
#include "server.h"
#include "log.h"
#include "time.h"
using namespace LR;
using namespace ps;


int main(int argc, char* argv[])
{
    ps::Start(0);
    startServer();
    clock_t start_t = clock();
    if (ps::IsWorker())
    {
        time_t start_t = 0, finish_t = 0;
        if(ps::MyRank() == 0)
            start_t = time(NULL);

        // std::string config("/home/caieleven/ML/LogisticRegression_v2/ScriptsAndDate/config.txt");
        std::string config(argv[1]);
        LR::LogReg<float> logreg(config);
        logreg.Train();
        // if(ps::MyRank() == 0)
        //     logreg.Test();
        if(ps::MyRank() == 0)
        {
            finish_t = time(NULL);
            Log::Info("Compute time: %f s\n", difftime(finish_t, start_t));
        }       
    }
    ps::Finalize(0, true);
    
    return 0;
}