#include "io.h"

namespace LR
{

LocalStream::LocalStream(const std::string &path, FileOpenMode mode) : path_(path)
{
    std::string mode_str;
    switch (mode)
    {
    case FileOpenMode::Read:
        mode_str = "r";
        break;
    case FileOpenMode::Write:
        mode_str = "w";
        break;
    case FileOpenMode::Append:
        mode_str = "a";
        break;
    case FileOpenMode::BinaryRead:
        mode_str = "rb";
        break;
    case FileOpenMode::BinaryWrite:
        mode_str = "wb";
        break;
    case FileOpenMode::BinaryAppend:
        mode_str = "ab";
    }
    fp_ = fopen(path_.c_str(), mode_str.c_str());
    if (fp_ == nullptr)
    {
        is_good_ = false;
        Log::Error("Faild to open LOcalStream %s\n", path_.c_str());
    }
    else
    {
        is_good_ = true;
    }
}

LocalStream::~LocalStream()
{
    is_good_ = false;
    if (fp_ != nullptr)
        std::fclose(fp_);
}

void LocalStream::Write(const void *buf, size_t size)
{
    if (std::fwrite(buf, 1, size, fp_) != size)
    {
        is_good_ = false;
        Log::Error("LocalStream.Write incomplete\n");
    }
}

size_t LocalStream::Read(void *buf, size_t size)
{
    return std::fread(buf, 1, size, fp_);
}

bool LocalStream::Good()
{
    return is_good_;
}

TextReader::TextReader(const std::string &path, size_t buf_size)
{
    stream_ = new LocalStream(path, FileOpenMode::Read);
    buf_size_ = buf_size;
    pos_ = length_ = 0;
    buf_ = new char[buf_size_];
}

size_t TextReader::GetLine(std::string &line)
{
    line.clear();
    bool isEnd = false;
    while (!isEnd)
    {
        while (pos_ < length_)
        {
            char &c = buf_[pos_++];
            if (c == '\n')
            {
                isEnd = true;
                break;
            }
            else
            {
                line += c;
            }
        }
        if (isEnd || LoadBuffer() == 0)
            break;
    }
    return line.size();
}

size_t TextReader::LoadBuffer()
{
    pos_ = length_ = 0;
    return length_ = stream_->Read(buf_, buf_size_ - 1);
}

TextReader::~TextReader()
{
    delete stream_;
    delete[] buf_;
}

}
