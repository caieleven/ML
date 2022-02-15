#include "io.h"

namespace LR
{

    FileOperator::FileOperator(const std::string &path, FileOpenMode mode):path_(path)
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
        }
        fp_ = fopen(path_.c_str(), mode_str.c_str());
        if (fp_ == nullptr)
        {
            is_good_ = false;
            Log::Error("Failed to open file %s\n", path_.c_str());
        }
        else
        {
            is_good_ = true;
        }
    }

    inline size_t FileOperator::Read(void *buf, size_t size)
    {
        return std::fread(buf, 1, size, fp_);
    }

    void FileOperator::Write(const void *buf, size_t size)
    {
        if(std::fwrite(buf, 1, size, fp_) != size)
        {
            is_good_ = false;
            Log::Error("Write date to file %s incomplete\n", path_.c_str());
        }
    }

    inline bool FileOperator::Good()
    {
        return is_good_;
    }

    FileOperator::~FileOperator()
    {
        is_good_ = false;
        if(fp_ != nullptr)
            std::fclose(fp_);
    }

    TextReader::TextReader(const std::string &path, size_t buf_size)
    {
        op_ = new FileOperator(path, FileOpenMode::Read);
        buf_size_ = buf_size;
        pos_ = length_ = 0;
        buf_ = new char[buf_size_];
    }

    size_t TextReader::GetLine(std::string &line)
    {
        line.clear();
        bool isEnd = false;
        while(!isEnd)
        {
            while(pos_ < length_)
            {
                char &c = buf_[pos_++];
                if(c == '\n')
                {
                    isEnd = true;
                    break;
                }
                else
                {
                    line += c;
                }
            }
            //when pos_ equals length_ and isEnd equals false, we need load new data
            //if there is no data to read, break;
            if(isEnd || LoadBuffer() == 0)
                break;
        }
        return line.size();
    }

    size_t TextReader::LoadBuffer()
    {
        pos_ = length_ = 0;
        return length_ = op_->Read(buf_, buf_size_ - 1);
    }
    
    TextReader::~TextReader()
    {
        delete op_;
        delete [] buf_;
    }


}