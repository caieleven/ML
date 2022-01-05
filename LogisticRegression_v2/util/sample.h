#ifndef SAMPLE_H
#define SAMPLE_H
#include "configure.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include "io.h"
#include "datatype.h"


namespace LR
{

    template <typename ElemType>
    class SampleReader
    {
    public:
        SampleReader(Configure& config);
        ~SampleReader();
        /**
         * @brief load data to the buffer
         * @param buffer_size the data size you want to load
         * @param buffer put line pointer in buffer
         * @return the true rows actually load
         */
        int GetSample(int buffer_size, Sample<ElemType>** buffer);
        void Free(int row_num);
    private:
        Read();
        /**
         * @param str string where data stored
         * @param idx the index of buffer_
         */
        void ParseLine(std::string& str, int idx);
        
        std::thread* th_;
        size_t buffer_size;
        int length_;
        int readlength_;
        int start_;
        int end_;
        std::string file_;
        TextReader* reader_;
        Sample<ElemType>** buffer_;
        std::mutex mutex_;
        std::condition_variable cv_;
    };
}

#endif