#include "samplereader.h"
#include <string>
#include <iostream>

using namespace LR;
using namespace std;







int main()
{
    std::string file("/home/caieleven/ML/DataSet/hd.txt");


    int readnum = 1000;
    int dimention = 13;
    int batch_size = 10;
    Sample<float> **batchsamples = new Sample<float>*[batch_size];
    SampleReader<float> reader(file, readnum, dimention);
    
    while(!reader.IsEndOfFile())
    {cout << "hhh" << endl;
        reader.GetSample(batch_size, batchsamples);
        printf("%d\n", batchsamples[0]->label);
    }
    return 0;
}