#include "configure.h"
#include "io.h"
#include "datatype.h"
#include "regular.h"
#include "log.h"
#include <math.h>
#include <vector>

namespace LR
{
    template <typename ElemType>
    class Objective
    {
    public:
        Objective(const Configure &config);
        virtual ~Objective();
        /**
         * @brief 获取configure file中指定的objective类
         * @param config configure file
         * @return 执行objective的指针
         */
        static Objective<ElemType> *Get(const Configure &config);
        /**
         * @brief 计算损失函数值
         * @param sample 样本指针
         * @param hvalue 假设函数值
         * @return 损失函数值
         */
        inline virtual float Loss(Sample<ElemType> *sample, float &hvalue);
        inline virtual void Gradient(Sample<ElemType> *sample, std::vector<ElemType> &model, std::vector<ElemType> &gradient);

    protected:
        inline float MathLog(float x);
        int input_dimention_;
        int output_dimention_;
        Regular<ElemType> *regular_;
    };

    template <typename ElemType>
    class SigmoidObjective : public Objective<ElemType>
    {
    public:
        SigmoidObjective(const Configure &config);
        // virtual ~SigmoidObjective();
        inline virtual float Loss(Sample<ElemType> *sample, float &hvalue) override;
        inline virtual void Gradient(Sample<ElemType> *sample, std::vector<ElemType> &model, std::vector<ElemType> &gradient) override;
        inline float Sigmoid(Sample<ElemType> *sample, std::vector<ElemType> &model);
    };

    template <typename ElemType>
    class SoftmaxObjective : public Objective<ElemType>
    {
    public:
        SoftmaxObjective(const Configure &config);
        // virtual ~SoftmaxObjective();
        inline virtual void Gradient(Sample<ElemType> *sample, std::vector<ElemType> &model, std::vector<ElemType> &gradient) override;
        inline virtual float Loss(Sample<ElemType> *sample, float &hvalue) override;
    };

    // base
    template <typename ElemType>
    Objective<ElemType> *Objective<ElemType>::Get(const Configure &config)
    {
        const std::string type = config.objective_type;
        Log::Info("Objective type is %s\n", config.objective_type.c_str());
        if (type == "sigmoid")
        {
            return new SigmoidObjective<ElemType>(config);
        }
        else if (type == "softmax")
        {
            return new SoftmaxObjective<ElemType>(config);
        }
        else
        {
            return new Objective<ElemType>(config);
        }
    }

    template <typename ElemType>
    Objective<ElemType>::Objective(const Configure &config) : input_dimention_(config.input_dimention),
                                                              output_dimention_(config.output_dimention)
    {
        regular_ = Regular<ElemType>::Get(config);
    }

    template <typename ElemType>
    Objective<ElemType>::~Objective()
    {
        delete regular_;
    }

    template <typename ElemType>
    inline void Objective<ElemType>::Gradient(Sample<ElemType> *sample, std::vector<ElemType> &model, std::vector<ElemType> &gradient)
    {
    }

    template <typename ElemType>
    inline float Objective<ElemType>::MathLog(float x)
    {
        return log(x < 0.000001f ? 0.000001f : x);
    }

    template <typename ElemType>
    inline float Objective<ElemType>::Loss(Sample<ElemType> *sample, float &hvalue)
    {
        return 0;
    }

    // Sigmoid

    template <typename ElemType>
    SigmoidObjective<ElemType>::SigmoidObjective(const Configure &config) : Objective<ElemType>(config)
    {
    }

    template <typename ElemType>
    inline void SigmoidObjective<ElemType>::Gradient(Sample<ElemType> *sample, std::vector<ElemType> &model, std::vector<ElemType> &gradient)
    {
        // Log::Debug("sample[0]=%f\n", sample->features[0]);
        //计算假设函数函数值
        float hvalue = Sigmoid(sample, model);
        Log::Debug("sample label=%d\n", sample->label);
        //Log::Debug("hvalue=%f\n", hvalue);
        //计算损失值并输出
        Log::Debug("loss = %f\n", Loss(sample, hvalue));
        //计算误差
        hvalue -= sample->label;
        //添加正则化项
        for (size_t i = 0; i < this->input_dimention_; ++i)
        {
            gradient[i] += hvalue * (sample->features[i]) + this->regular_->AddTerm(i, model);
        }
    }

    template <typename ElemType>
    float SigmoidObjective<ElemType>::Sigmoid(Sample<ElemType> *sample, std::vector<ElemType> &model)
    {
        //Log::Debug("lf=%lf\n", 1.0f + exp(-Dot(model, sample)));
        //Log::Debug("dot=%f\n", 1.0f / (1.0f + exp(-Dot(model, sample))));
        return static_cast<float>(1.0f / (1.0f + exp(-Dot(model, sample))));
    }

    template <typename ElemType>
    inline float SigmoidObjective<ElemType>::Loss(Sample<ElemType> *sample, float &hvalue)
    {
        if (sample->label == 1)
            return -this->MathLog(hvalue);
        else
            return -this->MathLog(1.0f - hvalue);
    }

    // Softmax
    template <typename ElemType>
    SoftmaxObjective<ElemType>::SoftmaxObjective(const Configure &config) : Objective<ElemType>(config)
    {
    }

    template <typename ElemType>
    inline void SoftmaxObjective<ElemType>::Gradient(Sample<ElemType> *sample, std::vector<ElemType> &model, std::vector<ElemType> &gradient)
    {
        // //计算损失函数值
        // float hvalue = Sigmoid(sample, model);
        // //计算误差
        // hvalue -= sample->label;
        // //添加正则化项
        // for(size_t i = 0; i < input_dimention_; ++i)
        // {
        //     gradient[i] += hvalue * (sample->label[i]) + this->regular_->AddTerm(i, model);
        // }
    }

    template <typename ElemType>
    inline float SoftmaxObjective<ElemType>::Loss(Sample<ElemType> *sample, float &hvalue)
    {
        return 0;
    }

}
