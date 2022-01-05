#ifndef DATATYPE_H
#define DATATYPE_H
#include <vector>

namespace LR
{
    template <typename ElemType>
    struct Sample
    {
        int label;
        std::vector<ElemType> features_;
        Sample(size_t size)
        {
            features_.reserve(size);
        }
    };

    /**
     * @brief 
     * @param num {int} the sample number
     * @param size (size_t} size of sample.features
     * @return the pointer of buffer
     */
    template <typename ElemType>
    Sample<ElemType> **CreateSampleBuff(int num, size_t size)
    {
        Sample<ElemType> samplep = new Sample<ElemType> *[num];
        for (int i = 0; i < size; ++i)
        {
            samplep[i] = new Sample(size)
        }
    }

}

#endif