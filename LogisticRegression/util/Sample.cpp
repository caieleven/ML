#include "Sample.h"
Sample::Sample(const char *dataPath, int rowNum, int colNum, int batchsize) : batch_size(batchsize)
{

    ifstream file;
    Data tempdata;
    tempdata.features.resize(colNum);

    file.open(dataPath);

    if (rowNum != -1)
    {
        float tempfeature;
        for (int i = 0; i < rowNum; i++)
        {
            if (file.eof())
            {
                cout << "the row number of the file is error" << endl;
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
        }
    }
    else
    {
        string line, tempstr;
        while (getline(file, line))
        {
            istringstream record(line);
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
}

bool Sample::loadNextMinibatchSample()
{
    batch_data.clear();
    if (_progress + batch_size > _samples.size())
        return false;

    auto i1 = _samples.begin() + _progress, i2 = _samples.begin() + _progress + batch_size;
    batch_data.assign(i1, i2);
    _progress += batch_size;
    return true;
}
