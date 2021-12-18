#ifndef IO_H
#define IO_H
#include "util/log.h"
#include 



namespace LR
{
    enum class FileOpenMode:int
    {
        Write = 0,
        Read = 1,
        Append
    };

    class FileOperator
    {
    public:
        FileOperator(const std::string& path, FileOpenMode mode);
        ~FileOperator();

        void Write(const void *buf, size_t size);
        
        /*
         * @return the true size of data
         */
        size_t Read(void *buf, size_t sizse);

        bool Good();
    
    private:
        bool is_good_;
        FILE *fp_;
        std::string path_;
    };


    class TextReader
    {
    public:
        TextReader(const std::string &path, size_t buf_size);
        ~TextReader();
        size_t GetLine(std::string &line);
    private:
        size_t LoadBuffer();
        char* buf_;
        size_t pos_, buf_size_, length_;
        FileOperator* op_;
    };



}






#endif