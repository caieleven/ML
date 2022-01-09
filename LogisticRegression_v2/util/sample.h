#ifndef SAMPLE_H
#define SAMPLE_H
#include "configure.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <sstream>
#include "io.h"
#include "log.h"
#include "datatype.h"

namespace LR
{
    template <typename ElemType>
    class SampleReader
    {
    public:
        SampleReader(std::string filepath, int read_buffer_size, int input_dimention);
        ~SampleReader();
        /**
         * @brief load data to the buffer, 
         * @param buffer_size the data size you want to load
         * @param buffer put line pointer in buffer
         * @return the true rows actually load
         */
        int GetSample(int buffer_size, Sample<ElemType> **buffer);
        void Free(int row_num);
        void Reset();
        void Stop();
        bool IsEndOfFile() const;

    private:
        void Read();
        /**
         * @param str string where data stored
         * @param idx the index of buffer_
         */
        void ParseLine(std::string &str, int idx);

        std::thread *th_;
        bool stop_;
        bool eof_;
        int buffer_size_;
        int input_dimention_;
        int length_;
        int readlength_;
        int start_;
        int end_;
        std::string file_;
        TextReader *reader_;
        Sample<ElemType> **buffer_;
        std::mutex mutex_;
        std::condition_variable cv_;
    };

    template <typename ElemType>
    /**
     * @brief reach a loop
     * @return if(a > b) retrun a-b, else return a
     */
    inline ElemType round(ElemType a, ElemType b)
    {
        return a >= b ? (a - b) : a;
    }

    template <typename ElemType>
    SampleReader<ElemType>::SampleReader(std::string filepath, int read_buffer_size, int input_dimention) : stop_(false),
                                                                                                            eof_(false),
                                                                                                            file_(filepath),
                                                                                                            buffer_size_(read_buffer_size),
                                                                                                            input_dimention_(input_dimention),
                                                                                                            length_(0),
                                                                                                            readlength_(0),
                                                                                                            start_(0),
                                                                                                            end_(0)
    {
        buffer_ = CreateSampleBuff<ElemType>(buffer_size_, input_dimention + 1);
        reader_ = new TextReader(file_);
        Log::Info("SampleReader begin to read data from %s\n", file_.c_str());
        th_ = new std::thread(&SampleReader<ElemType>::Read, this);
    }

    template <typename ElemType>
    SampleReader<ElemType>::~SampleReader()
    {
        DeleteSampleBuff<ElemType>(buffer_, buffer_size_);
        th_->join();
        delete buffer_;
        delete reader_;
        delete th_;
    }

    template <typename ElemType>
    void SampleReader<ElemType>::Free(int row_num)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            length_ -= row_num;
            readlength_ -= row_num;
        }
        cv_.notify_one();
    }

    template <typename ElemType>
    int SampleReader<ElemType>::GetSample(int buffer_size, Sample<ElemType> **buffer)
    {
        //Log::Debug("begin to load data from SampleReader\n");
        int size;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            size = length_ - readlength_;
            size = size > buffer_size ? buffer_size : size;
            readlength_ += size;
        }
        for (int i = 0; i < size; ++i)
        {
            buffer[i] = buffer_[round(start_ + i, buffer_size_)];
        }
        start_ = round(start_ + size, buffer_size_);
        return size;
    }

    template <typename ElemType>
    void SampleReader<ElemType>::Read()
    {
        while (1)
        {
            while (eof_ && !stop_)
            {
                Log::Info("file %s read end, read thread sleep\n", file_.c_str());
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
            std::string line;
            std::unique_lock<std::mutex> lock(mutex_);
            while (reader_->GetLine(line) && (!stop_))
            {
                if (length_ == buffer_size_)
                {
                    cv_.wait(lock);
                }
                ParseLine(line, end_);
                ++length_;
                end_ = round(++end_, buffer_size_);
            }
            eof_ = true;
            lock.unlock();
            if (stop_)
                break;
        }
    }

    template <typename ElemType>
    void SampleReader<ElemType>::ParseLine(std::string &str, int idx)
    {
        std::stringstream ss(str);
        Sample<ElemType> *data = buffer_[idx];
        data->features.clear();
        ss >> data->label;
        ElemType feature;
        while (ss >> feature)
            data->features.emplace_back(feature);
        data->features.emplace_back(1);
    }

    template <typename ElemType>
    void SampleReader<ElemType>::Reset()
    {
        //std::lock_guard lock(mutex_);
        stop_ = false;
        eof_ = false;
        start_ = 0;
        end_ = 0;
        length_ = 0;
        readlength_ = 0;
        delete reader_;
        reader_ = new TextReader(file_);
    }

    template <typename ElemType>
    void SampleReader<ElemType>::Stop()
    {
        stop_ = true;
    }

    template <typename ElemType>
    inline bool SampleReader<ElemType>::IsEndOfFile() const
    {
        return (eof_ && length_ == readlength_);
    }

}

#endif