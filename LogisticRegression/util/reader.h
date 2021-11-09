#ifndef UTIL_READER_H
#define UTIL_READER_H

#include <string>
#include "log.h"
#include "Sample.h"
#include "io.h"


namespace LR
{

class SampleReader
{
public:
    SampleReader(const std::string& files, 
                size_t row_size, 
                int output_size,
                int max_row_buffer_count, 
                bool init,
                int update_per_sample);
    ~SampleReader();
    int Read(int num_row, Data **buffer);
    void Free(int num_row);
    int Ask();
    inline bool EndOfFile() const
    {
        return eof_;
    }
private:
    void Main();


private:
    Data** buffer_;
    bool eof_;
    int start_;
    int length_;
    int end_;
    int read_length_;
    int output_size_;
    int buffer_size_;
    TextReader *reader_;
    size_t row_size_;
    size_t sample_batch_size_;
    size_t sample_count_;


};





}


#endif