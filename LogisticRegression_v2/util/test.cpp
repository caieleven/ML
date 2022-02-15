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
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    int cn = 0;
    while(!reader.IsEndOfFile())
    {
        cout << "hhh" << endl;
        int count = reader.GetSample(batch_size, batchsamples);
        reader.Free(count);
        printf("%d\n", ++cn);
        cout << "bbb" << endl;
    }
    return 0;
}

// void myprint()
// {
//     cout << "hello world!" << endl;
// }


// int main()
// {
//     thread myjob(myprint);
//     myjob.join();
//     return 0;
// }