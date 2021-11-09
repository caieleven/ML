#ifndef UTIL_IO_H
#define UTIL_IO_H

#include <cstdio>
#include <string>
#include "log.h"
#include <cstddef>

namespace LR
{

enum class FileOpenMode :int {
  Write = 0,
  Read = 1,
  Append = 2,
  BinaryWrite = 3,
  BinaryRead = 4,
  BinaryAppend = 5
};

class LocalStream
{
public:
    LocalStream(const std::string& path, FileOpenMode mode);
    ~LocalStream();

    /* 
     * @brief write something to a file
     * @param buf pointer to a memory buffer
     * @param size data size
     */
    void Write(const void *buf, size_t size);

    /*
     * @brief read data from a file
     * @param buf pointer to a memory buffer
     * @param size data size
     * @return the size of data read
     */
    size_t Read(void *buf, size_t size);
    bool Good();
private:
    bool is_good_;
    FILE *fp_;
    std::string path_;
};

class TextReader {
public:
  TextReader(const std::string &path, size_t buf_size);

  size_t GetLine(std::string &line);

  ~TextReader();
private:
  size_t LoadBuffer();

  char* buf_;
  size_t pos_, buf_size_, length_;
  LocalStream* stream_;
};


}

#endif