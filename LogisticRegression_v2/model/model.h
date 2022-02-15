#include "configure.h"
#include "samplereader.h"
#include "objective.h"
#include "log.h"
#include <string.h>
#include <sstream>

namespace LR
{
    template <typename ElemType>
    class Model
    {
    public:
        Model(const Configure &config);
        virtual ~Model();
        virtual void Train(int num, Sample<ElemType> **samples);
        inline virtual void Update();
        /**
         * @brief 将模型权重以文本格式存储
         * @param config 其中包含存放model的文件路径
         */
        virtual void Store(const Configure &config) const;
        // /**
        //  * @brief 读取文本文件中的模型
        //  * @param config 其中包含存放model的文件路径
        //  */
        // virtual void Load(const Configure& config);
        static Model<ElemType> *Get(const Configure &config);

    protected:
        Objective<ElemType> *objective_;
        std::vector<ElemType> weight_;
        std::vector<ElemType> delta_;
        int update_count_;
        int mini_batch_size_;
        float alpha_;
        float alpha_coef_;
    };

    // template <typename ElemType>
    // class PSModel : public Model<ElemType>
    // {
    // public:
    //     PSModel(const Configure &config);
    //     //virtual ~PSModel();
    //     virtual void Train(int num, Sample<ElemType> **samples);
    //     inline virtual void Update();
    // };

    template <typename ElemType>
    Model<ElemType> *Model<ElemType>::Get(const Configure &config)
    {
        if (config.use_ps)
            return new Model<ElemType>(config);
        else
            return new Model<ElemType>(config);
    }

    template <typename ElemType>
    Model<ElemType>::Model(const Configure &config)
    {
        Log::Info("Init local model\n");
        size_t size = config.input_dimention * config.output_dimention;
        objective_ = Objective<ElemType>::Get(config);
        weight_.reserve(size);
        delta_.reserve(size);
        //初始化，很重要，容易漏掉
        for(size_t i = 0; i < size; ++i)
        {
            weight_.emplace_back(0);
            delta_.emplace_back(0);
        }
        update_count_ = 0;
        mini_batch_size_ = config.mini_batch_size;
        alpha_ = config.learning_rate;
        alpha_coef_ = 0.01;
    }

    template <typename ElemType>
    Model<ElemType>::~Model()
    {
        delete objective_;
    }

    template <typename ElemType>
    void Model<ElemType>::Train(int num, Sample<ElemType> **samples)
    {
        Log::Info("Train with %d samples\n", num);
        for (int i = 0; i < num; i += mini_batch_size_)
        {
            Log::Debug("Train model[1] = %f model.size=%d\n", weight_[1], weight_.size());
            Log::Debug("Train delta[1] = %f delta.size=%d\n", delta_[1], delta_.size());
            delta_.clear();
            int upper = i + mini_batch_size_;
            upper = upper > num ? num : upper;
            for (int j = i; j < upper; ++j)
                objective_->Gradient(samples[j], weight_, delta_);
            int batch_size = upper - i;
            if (batch_size > 1)
                for (size_t k = 0; k < delta_.size(); ++k)
                    delta_[k] = static_cast<ElemType>(delta_[k] / static_cast<double>(batch_size));
            this->Update();
        }
    }

    template <typename ElemType>
    inline void Model<ElemType>::Update()
    {
        ++update_count_;
        float talpha = 1.0 / (1 + alpha_coef_ * update_count_) * alpha_;
        alpha_ = talpha > 0.001 ? talpha : 0.001;
        Log::Debug("alpha_ = %f\n", alpha_);
        // Log::Debug("updata size=%d\n", weight_.size());
        for (size_t i = 0; i < weight_.size(); ++i)
        {
            weight_[i] -= alpha_ * delta_[i];
            Log::Debug("jisuanchengfa\n");
            Log::Debug("alpha_ * delta = %f\n", alpha_ * delta_[1]);
            Log::Debug("model[1] = %f\n", weight_[1]);
            Log::Debug("delta[1] = %f\n", delta_[1]);
        }
    }

    template <typename ElemType>
    void Model<ElemType>::Store(const Configure &config) const
    {
        Log::Info("Store Model to %s\n", config.model_file.c_str());
        FileOperator fp(config.model_file, FileOpenMode::Write);
        std::string str;
        std::stringstream ss;
        for (size_t i = 0; i < weight_.size(); ++i)
        {
            // str += std::to_string(weight_[i]);
            // str += " ";
            ss << weight_[i] << " ";
        }
        ss << "\n";
        fp.Write(ss.str().c_str(), ss.str().length());
    }
}