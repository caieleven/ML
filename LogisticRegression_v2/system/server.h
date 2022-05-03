/*
 * @Author: Baorun Chen
 * @Date: 2022-03-07 23:09:40
 * @LastEditTime: 2022-05-03 11:38:10
 * @Description:
 */
#include "ps/ps.h"
#include "log.h"

namespace LR
{

    template <typename Val>
    struct KVServerHandle
    {
        void operator()(const ps::KVMeta &req_meta, const ps::KVPairs<Val> &req_data, ps::KVServer<Val> *server)
        {
            size_t n = req_data.keys.size();
            ps::KVPairs<Val> res;
            if (req_meta.push)
            {
                CHECK_EQ(n, req_data.vals.size());
                // Log::Write(LogLevel::Debug,"push gradient[1]:%f\n", req_data.vals[1]);
            }
            else
            {
                res.keys = req_data.keys;
                res.vals.resize(n);
                // Log::Write(LogLevel::Debug, "pull weight[1]:%f\n", store[1]);
            }
            for (size_t i = 0; i < n; ++i)
            {
                ps::Key key = req_data.keys[i];
                if (req_meta.push)
                {
                    store[key] -= req_data.vals[i];
                }
                else
                {
                    res.vals[i] = store[key];
                }
            }
            // if(req_meta.push)
            //     Log::Debug("After push, the weight[1]:%f\n", store[1]);
            server->Response(req_meta, res);
        }
        std::unordered_map<ps::Key, Val> store;
    };

    void startServer()
    {
        if (!ps::IsServer())
            return;
        auto server = new ps::KVServer<float>(0);
        server->set_request_handle(KVServerHandle<float>());
        //Log::Write(LogLevel::Debug, "Start Server!\n");
        // LR::Log::Info("Start server\n");
    }
}