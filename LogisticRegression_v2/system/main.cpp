#include "logreg.h"
#include "server.h"
#include "log.h"
#include "time.h"
#include <stdlib.h>
#include <sstream>
using namespace LR;
using namespace ps;


int main(int argc, char* argv[])
{
    ps::Start(0);
    startServer();
    system("pwd");
    if (ps::IsWorker())
    {
        // generate train data
        std::stringstream ss;
        std::string filepath("/code/ML/DataSet/dataproducer.py ");
        std::string datasize(argv[2]);
        ss << "python3 " << filepath << datasize << " " << ps::NumWorkers() << " " << ps::MyRank();
        system(ss.str().c_str());


        time_t start_t = 0, finish_t = 0;
        if(ps::MyRank() == 0)
        {
            start_t = time(NULL);
            // Log::Info("ServerNum = %d\tWorkerNum = %d\n", ps::NumServers(), ps::NumWorkers);
        }
            

        std::string config(argv[1]);
        LR::LogReg<float> logreg(config);
        logreg.Train();

        if(ps::MyRank() == 0)
        {
            finish_t = time(NULL);
            Log::Info("Compute time: %ld s\n", difftime(finish_t, start_t));
        }       
    }
    ps::Finalize(0, true);
    return 0;
}