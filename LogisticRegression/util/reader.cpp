#include "reader.h"

namespace LR
{

template <class ElemType>
inline ElemType min(ElemType a, ElemType b)
{
    return a < b ? a : b;
}

SampleReader::SampleReader(const std::string& files, 
                size_t row_size, 
                
                int output_size,
                int max_row_buffer_count, 
                bool init = true,
                int update_per_sample):
                eof_(true),
                reader_(nullptr),
                row_size_(row_size),
                output_size_(output_size),
                sample_batch_size_(update_per_sample),
                sample_count_(0)
{
    // buffer_ = new Data*[row_size_];
    // for(int i = 0; i < num; ++i)
    //     buffer_[i] = new Data
}




}