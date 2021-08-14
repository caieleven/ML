#ifndef SAMPLE_H
#define SAMPLE_H
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

//feature数为colnum-1
//最后一个feature为1

template <class ElemType>
class Sample
{
private:
    vector<vector<ElemType> > *_features;
    vector<int> *_label;
    int _rowNum;
    int _colNum;

public:
    Sample(const char* dataPath = NULL, int rowNum = 1, int colNum = 1);
    ~Sample();
    void getSampleByIndex(int index, vector<ElemType> &rowSample, int &label);
    int getDataRawNum();
    int getDataColNum();

};

template <class ElemType>
Sample<ElemType>::Sample(const char* dataPath, int rowNum, int colNum):_rowNum(rowNum), _colNum(colNum)
{
    ifstream file;
    int tempLabel;
    
    _features = new vector<vector<ElemType> >;
    _label = new vector<int>;
    file.open(dataPath);
    if(file.fail())
    {
        cout << "文件打开失败" << endl;
        file.close();
        return;
    }
    vector<ElemType> tempvec(_colNum);
    tempvec[_colNum - 1] = 1;
    //ifstream file(dataPath);
    for (int i = 0; i < _rowNum; i++)
    {
        for (int j = 0; j < _colNum; j++)
            file >> tempvec[j];


        _features->push_back(tempvec);
        file >> tempLabel;
        _label->push_back(tempLabel);
    }
    file.close();
}


template <class ElemType>
Sample<ElemType>::~Sample()
{
    delete _features;
    delete _label;
}


template <class ElemType>
int Sample<ElemType>::getDataColNum()
{
    return _colNum;
}


template <class ElemType>
int Sample<ElemType>::getDataRawNum()
{
    return _rowNum;
}

template <class ElemType>
void Sample<ElemType>::getSampleByIndex(int index, vector<ElemType> &rowSample, int &label)
{
    (*_features)[index].assign(rowSample.begin(), rowSample.end());
    label = (*_label)[index];
}


#endif