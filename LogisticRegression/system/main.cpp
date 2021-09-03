#include "LogisticRegression.h"
#include "ps/ps.h"
using namespace std;
using namespace ps;


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
        }
        else
        {
            res.keys = req_data.keys;
            res.vals.resize(n);
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
}


int main(int argc, char *argv[])
{
    ps::Start();
    startServer();
    if(ps::IsWorker())
    {
        LogisticRegression lr(argv[1]);
        lr.train();
    }
    Finalize();

    return 0;
}