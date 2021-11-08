#include "LogisticRegression.h"
#include "ps/ps.h"
#include "log.h"
using namespace std;
using namespace ps;
using namespace LR;

/*
 * @TODO rewrite the KVServerHandle ||
 *       add the starting script
 */


template <typename Val>
struct KVServerHandle
{
    void operator()(const KVMeta &req_meta, const KVPairs<Val> &req_data, KVServer<Val> *server)
    {
        size_t n = req_data.keys.size();
        KVPairs<Val> res;
        if (req_meta.push)
        {
            CHECK_EQ(n, req_data.vals.size());
            Log::Write(LogLevel::Debug,"push gradient[1]:%f\n", req_data.vals[1]);
        }
        else
        {
            res.keys = req_data.keys;
            res.vals.resize(n);
            Log::Write(LogLevel::Debug, "pull weight[1]:%f\n", store[1]);
        }
        for (size_t i = 0; i < n; ++i)
        {
            Key key = req_data.keys[i];
            if (req_meta.push)
            {
                store[key] += req_data.vals[i];
            }
            else
            {
                res.vals[i] = store[key];
            }
        }
        if(req_meta.push)
            Log::Debug("After push, the weight[1]:%f\n", store[1]);
        server->Response(req_meta, res);
    }
    std::unordered_map<Key, Val> store;
};


void startServer()
{
    if(!ps::IsServer())
        return;
    auto server = new ps::KVServer<float>(0);
    server->set_request_handle(KVServerHandle<float>());
    Log::Write(LogLevel::Debug, "Start Server!\n");
}

int main(int argc, char *argv[])
{
    ps::Start();
    startServer();
    if(ps::IsWorker())
    {
        Log::Write(LogLevel::Debug, "Start Worker!\n");
        LR::LogisticRegression lr(argv[1]);
        lr.train();
        //lr.predict();
        lr.saveModel();
    }
    Finalize();

    return 0;
}