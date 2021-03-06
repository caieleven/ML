#include "configure.h"
#include "samplereader.h"
#include "objective.h"
#include "log.h"
#include <string.h>
#include <sstream>
#include "ps/ps.h"

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
         * @param num 样本数量
         * @param samples 样本
         * @return 返回正确预测的样本数
         */
        inline virtual int Test(int num, Sample<ElemType>** samples);
        
        
        /**
         * @param sample 需要预测的样本
         * @return 预测结果
         */
        inline virtual int Predict(Sample<ElemType>* sample);

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
        Configure cconfig_;
    };

    template <typename ElemType>
    class PSModel : public Model<ElemType>
    {
    public:
        PSModel(const Configure &config);
        void Update();
        virtual void Train(int num, Sample<ElemType> ** samples);

    private:
        ps::KVWorker<float> *kv_;
        std::vector<ps::Key> keys_;
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
        {
            Log::Info("Initialize PS Model\n");
            return new PSModel<ElemType>(config);
        }
            
        else
        {
            Log::Info("Initialize Loacal Model\n");
            return new Model<ElemType>(config);
        }
            
    }

    template <typename ElemType>
    Model<ElemType>::Model(const Configure &config) : cconfig_(config)
    {
        // Log::Info("Init local model\n");
        size_t size = (config.input_dimention + 1) * config.output_dimention;
        objective_ = Objective<ElemType>::Get(config);
        weight_.reserve(size);
        delta_.reserve(size);
        //初始化，很重要，容易漏掉
        for (size_t i = 0; i < size; ++i)
        {
            weight_.emplace_back(0);
            delta_.emplace_back(0);
        }
        update_count_ = 0;
        mini_batch_size_ = config.mini_batch_size;
        alpha_ = config.learning_rate;
        alpha_coef_ = 0.00000;
    }

    template <typename ElemType>
    Model<ElemType>::~Model()
    {
        delete objective_;
    }

    template <typename ElemType>
    void Model<ElemType>::Train(int num, Sample<ElemType> **samples)
    {
        // Log::Info("Train with %d samples\n", num);
        for (int i = 0; i < num; i += mini_batch_size_)
        {
            // Log::Debug("Train model[1] = %f model.size=%d\n", weight_[1], weight_.size());
            // Log::Debug("Train delta[1] = %f delta.size=%d\n", delta_[1], delta_.size());
            for (size_t idx = 0; idx < delta_.size(); ++idx)
                delta_[idx] = 0;
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
    int Model<ElemType>::Test(int num, Sample<ElemType>** samples)
    {
        // Log::Info("Begin to test with %d samples\n", num);
        int cont = 0;
        for (int i = 0; i < num; ++i)
        {
            int cls = this->Predict(samples[i]);
            if(cls == samples[i]->label)
                ++cont;
        }
        return cont;
    }

    template <typename ElemType>
    inline int Model<ElemType>::Predict(Sample<ElemType>* sample)
    {
        return objective_->Predict(weight_, sample);
    }

    template <typename ElemType>
    inline void Model<ElemType>::Update()
    {
        ++update_count_;
        float talpha = 1.0 / (1 + alpha_coef_ * update_count_) * alpha_;
        alpha_ = talpha > 0.001 ? talpha : 0.001;
        // Log::Debug("alpha_ = %f\n", alpha_);
        // Log::Debug("updata size=%d\n", delta_.size());
        for (size_t i = 0; i < weight_.size(); ++i)
        {
            weight_[i] -= alpha_ * delta_[i];
        }
        Store(cconfig_);
    }

    template <typename ElemType>
    void Model<ElemType>::Store(const Configure &config) const
    {
        // Log::Info("Store Model to %s\n", config.model_file.c_str());
        FileOperator fp(config.model_file, FileOpenMode::Append);
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

    template <typename ElemType>
    PSModel<ElemType>::PSModel(const Configure &config) : Model<ElemType>(config)
    {
        size_t size = (config.input_dimention + 1) * config.output_dimention;
        kv_ = new ps::KVWorker<float>(0, 0);
        keys_.reserve(size);
        for (size_t i = 0; i < size; ++i)
        {
            keys_.push_back(i);
        }
    }

    template <typename ElemType>
    inline void PSModel<ElemType>::Update()
    {
        ++this->update_count_;
        float talpha = 1.0 / (1 + this->alpha_coef_ * this->update_count_) * this->alpha_;
        this->alpha_ = talpha > 0.001 ? talpha : 0.001;
        // Log::Debug("alpha_ = %f\n", this->alpha_);
        // Log::Debug("updata size=%d\n", delta_.size());
        for (size_t i = 0; i < this->delta_.size(); ++i)
        {
            this->delta_[i] = this->alpha_ * this->delta_[i];
        }
        kv_->Wait(kv_->Push(keys_, this->delta_));
    }

    template <typename ElemType>
    void PSModel<ElemType>::Train(int num, Sample<ElemType> **samples)
    {
        // Log::Info("Train with %d samples\n", num);
        for (int i = 0; i < num; i += this->mini_batch_size_)
        {
            kv_->Wait(kv_->Pull(keys_, &this->weight_));
            // Log::Debug("Train model[1] = %f model.size=%d\n", weight_[1], weight_.size());
            // Log::Debug("Train delta[1] = %f delta.size=%d\n", delta_[1], delta_.size());
            for (size_t idx = 0; idx < this->delta_.size(); ++idx)
                this->delta_[idx] = 0;
            int upper = i + this->mini_batch_size_;
            upper = upper > num ? num : upper;
            for (int j = i; j < upper; ++j)
                this->objective_->Gradient(samples[j], this->weight_, this->delta_);
            int batch_size = upper - i;
            if (batch_size > 1)
                for (size_t k = 0; k < this->delta_.size(); ++k)
                    this->delta_[k] = static_cast<ElemType>(this->delta_[k] / static_cast<double>(batch_size));
            this->Update();
        }
    }

}
