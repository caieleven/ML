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
        static Objective<ElemType> *Get(const Configure& config);
        inline virtual float Gradient(Sample<ElemType> *sample, std::vector<ElemType> &model, std::vector<ElemType> &gradient);

    protected:
        int input_dimention_;
        int output_dimention_;
        Regular<ElemType> *regular_;
    };

    template <typename ElemType>
    class SigmoidObjective : public Objective<ElemType>
    {
    public:
        SigmoidObjective(const Configure &config);
        virtual ~Objective();
        inline virtual float Gradient(Sample<ElemType> *sample, std::vector<ElemType> &model, std::vector<ElemType> &gradient);
        inline float Sigmoid(Sample<ElemType>* sample, std::vector<ElemType>& model);
    };

    template <typename ElemType>
    class SoftmaxObjective : public Objective<ElemType>
    {
    public:
        SoftmaxObjective(const Configure &config);
        virtual ~Objective();
        inline virtual float Gradient(Sample<ElemType> *sample, std::vector<ElemType> &model, std::vector<ElemType> &gradient);
    };
    
    
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
    Objective<ElemType>::Objective(const Configure& config):input_dimention_(config.input_dimention),
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
    SigmoidObjective<ElemType>::SigmoidObjective(const Configure& config):Objective<ElemType>(config)
    {
    }

    template <typename ElemType>
    float SigmoidObjective<ElemType>::Gradient(Sample<ElemType> *sample, std::vector<ElemType> &model, std::vector<ElemType> &gradient)
    {
        //计算损失函数值
        float hvalue = Sigmoid(sample, model);
        //计算误差
        hvalue -= sample->label;
        //添加正则化项
        for(size_t i = 0; i < input_dimention_; ++i)
        {
            gradient[i] += hvalue * (sample->label[i]) + regular_->AddTerm(i, model);
        }
    }

    template <typename ElemType>
    float SigmoidObjective<ElemType>::Sigmoid(Sample<ElemType>* sample, std::vector<ElemType>& model)
    {
        return static_cast<float>(1.0f / (1.0f + exp(-Dot(model, sample))));
    }

        template <typename ElemType>
    SoftmaxObjective<ElemType>::SoftmaxObjective(const Configure& config):Objective<ElemType>(config)
    {
    }

    template <typename ElemType>
    float SoftmaxObjective<ElemType>::Gradient(Sample<ElemType> *sample, std::vector<ElemType> &model, std::vector<ElemType> &gradient)
    {
        //计算损失函数值
        float hvalue = Sigmoid(sample, model);
        //计算误差
        hvalue -= sample->label;
        //添加正则化项
        for(size_t i = 0; i < input_dimention_; ++i)
        {
            gradient[i] += hvalue * (sample->label[i]) + regular_->AddTerm(i, model);
        }
    }

}
