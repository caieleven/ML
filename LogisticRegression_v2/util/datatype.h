#ifndef DATATYPE_H
#define DATATYPE_H
#include <vector>

namespace LR
{
    template <typename ElemType>
    struct Sample
    {
        int label;
        std::vector<ElemType> features;
        Sample(int size)
        {
            features.reserve(size);
        }
    };

    /**
     * @brief
     * @param num {int} the sample number
     * @param size (size_t} size of sample.features
     * @return the pointer of buffer
     */
    template <typename ElemType>
    Sample<ElemType> **CreateSampleBuff(int num, int size)
    {
        Sample<ElemType> **samplep = new Sample<ElemType> *[num];
        for (int i = 0; i < num; ++i)
        {
            samplep[i] = new Sample<ElemType>(size);
        }
        return samplep;
    }

    template <typename ElemType>
    void DeleteSampleBuff(Sample<ElemType> **samplep, int num)
    {
        for (int i = 0; i < num; i++)
            delete samplep[i];
    }

    /**
     * @brief 点乘
     * @param model 模型引用
     * @param sample 样本指针
     * @return 返回计算结果，为float型
     */
    template <typename ElemType>
    inline float Dot(std::vector<ElemType> &model, Sample<ElemType> *sample)
    {
        float sum = 0;
        int size = model.size();
        for (int i = 0; i < size; ++i)
        {
            sum += model[i] * sample->features[i];
        }
        return sum;
    }
}

#endif