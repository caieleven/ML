#include "Sample.h"

namespace LR
{

    Sample::Sample(const char *dataPath, int rowNum, int colNum, int batchsize) : batch_size(batchsize)
    {

        std::ifstream file;
        Data tempdata;
        //tempdata.features.resize(colNum);
        Log::Write(LogLevel::Debug, "start to read sample file:%s\n", dataPath);

        file.open(dataPath);
        if(file)
            Log::Write(LogLevel::Debug, "open sample file %s successfully\n", dataPath);
        else
            Log::Write(LogLevel::Debug, "open faild\n");
        if (rowNum != -1)
        {
            float tempfeature;
            for (int i = 0; i < rowNum; i++)
            {
                if (file.eof())
                {
                    Log::Write(LogLevel::Fatal, "the row number of the file is essor\n");
                    break;
                }
                file >> tempdata.label;
                tempdata.features.push_back(1); //the first feature is 1
                for (int j = 0; j < colNum; j++)
                {
                    file >> tempfeature;
                    tempdata.features.push_back(tempfeature);
                }
                _samples.push_back(tempdata);
                //Log::Debug("%d\n", tempdata.features.size());
                tempdata.features.clear();
            }
            
        }
        else
        {
            //Bug::for some reasons, after reading 842 lines, the program stops running, but doesn't exit.
            int cnt = 0;
            std::string line, tempstr;
            while (getline(file, line))
            {
                Log::Write(LogLevel::Debug, "read %d line\n", ++cnt);
                std::istringstream record(line);
                record >> tempstr;
                tempdata.label = atoi(tempstr.c_str());
                while (record >> tempstr)
                {
                    tempdata.features.push_back(atof(tempstr.c_str()));
                }
                _samples.push_back(tempdata);
            }
        }

        file.close();
        Log::Write(LogLevel::Debug, "read sample file %s successfully\n", dataPath);
    }

    bool Sample::loadNextMinibatchSample()
    {

        batch_data.clear();
        batch_data.resize(batch_size);
        if (_progress + batch_size > _samples.size())
            return false;
        Log::Write(LogLevel::Debug, "load sample [ %lld, %lld ]\n", _progress, _progress + batch_size);
        //auto i1 = _samples.begin() + _progress, i2 = _samples.begin() + _progress + batch_size;
        //auto i1 = _samples.begin(), i2 = _samples.begin();
        auto i1 = _samples.begin(), i2 = _samples.begin();
        for(int i = 0; i < _progress; ++i)
            i1++;
        for(int j = 0; j < _progress + batch_size; ++j)
            i2++;
        batch_data.assign(i1, i2);
        Log::Debug("the 1st line:\n");
        for(int i = 0; i < batch_data[0].features.size();++i)
            printf("%f ", batch_data[0].features[i]);
        Log::Debug("the 1st line end\n");
        _progress += batch_size;
        return true;
    }

}