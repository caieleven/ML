#ifndef IO_H
#define IO_H
#include "log.h"
#include <string>

namespace LR
{
    enum class FileOpenMode:int
    {
        Write = 0,
        Read = 1,
        Append = 2
    };

    class FileOperator
    {
    public:
        FileOperator(const std::string& path, FileOpenMode mode);
        ~FileOperator();

        void Write(const void *buf, size_t size);
        
        /*
         * @param buf pointer to a memory buffer
         * @param size data size
         * @return the true size of data
         */
        inline size_t Read(void *buf, size_t size);

        inline bool Good();
    
    private:
        bool is_good_;
        FILE *fp_;
        std::string path_;
    };


    class TextReader
    {
    public:
        TextReader(const std::string &path, size_t buf_size = 1024);
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