#include "reader.h"

namespace LR
{

template <class ElemType>
inline ElemType min(ElemType a, ElemType b)
{
    return a < b ? a : b;
}

inline int round(int cur, int size) 
{
    return (cur >= size) ? (cur - size) : cur;
}

SampleReader::SampleReader(const std::string& files, 
                size_t row_size, 
                int output_size,
                int max_row_buffer_count, 
                //bool init = true,
                int update_per_sample):
                eof_(true),
                files_(files),
                reader_(nullptr),
                row_size_(row_size),
                buffer_size_(max_row_buffer_count*3),
                output_size_(output_size),
                sample_batch_size_(update_per_sample),
                sample_count_(0)
{
    buffer_ = new Data*[buffer_size_];
    for(int i = 0; i < buffer_size_; ++i)
        buffer_[i] = new Data();
    start_ = end_ = 0;
    length_ = read_length_ = 0;
    eof_ = false;
    Main();
}

void SampleReader::Main()
{
    reader_ = new TextReader(files_, 1024);
    std::string line;
    Log::Debug("SampleReader start to read file %s\n", files_.c_str());
    while(reader_->GetLine(line))
    {
        ParseLine(line, end_);
        ++length_;
        ++end_;
        if(length_ >= buffer_size_)
            break;
        
    }
    Log::Debug("end=%d, length=%d\n", end_, length_);
    Log::Debug("SampleReader end of file %s\n", files_.c_str());
}


void SampleReader::ParseLine(const std::string& line, int idx)
{
    Data *data = this->buffer_[idx];
    data->features.clear();
    std::stringstream iss(line);
    iss >> data->label;
    float value;
    while(iss >> value)
    {
        data->features.push_back(value);
    }
    data->features.push_back(1);
}


int SampleReader::Read(int num_row, Data **buffer)
{
    int size = min<int>(length_-read_length_, num_row);
    read_length_ += size;
    for(int i = 0; i < size; ++i)
    {
        //buffer[i] = buffer_[round(start_ + i, buffer_size_)];
        buffer[i] = buffer_[round(start_ + i, length_)];
    }
    start_ = round(start_ + size, length_);
    return size;
}


SampleReader::~SampleReader()
{
    delete reader_;
    for(int i = 0; i < buffer_size_; ++i)
    {
        delete buffer_[i];
    }
    delete buffer_;

}



}