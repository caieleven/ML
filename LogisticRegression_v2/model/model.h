 #include "configure.h"
 #include "samplereader.h"
 #include "objective.h"
 #include "log.h"

namespace LR
{
    template <typename ElemType>
    class Model
    {
    public:
        Model(const Configure& config);
        virtual ~Model();
        virtual void Train(int num, Sample<ElemType>** samples);
        inline virtual void Update();
        // /**
        //  * @brief 将模型权重以文本格式存储
        //  * @param model_file 存储路径
        //  */        
        // virtual void Store(const std::string& model_file);
        // /**
        //  * @brief 读取文本文件中的模型
        //  * @param model_file
        //  */        
        // virtual void Load(const std::string& model_file);
        static Model<ElemType>* Get(const Configure& config);

    protected:
        Objective<ElemType>* objective_;
        std::vector<ElemType> weight_;
        std::vector<ElemType> delta_;
        int mini_batch_size_;
        float alpha_;
    };

    template <typename ElemType>
    Model<ElemType>::Model(const Configure& config)
    {
        Log::Info("Init local model\n");
        size_t size = config.input_dimention * config.output_dimention;
        weight_.reserve(size);
        delta_.reserve(size);
        objective_ = Objective<ElemType>::Get(config);
    }

    template <typename ElemType>
    void Model<ElemType>::Train(int num, Sample<ElemType>** samples)
    {
        for(int i = 0; i < num; i += mini_batch_size_)
        {
            delta_ -> clear();
            int upper = i + mini_batch_size_;
            upper = upper > num ? num : upper;
            for (int j = i; j < upper; ++j)
                objective_->Gradient(samples[j], weight_, delta_); 
            int batch_size = upper - i;
            if(batch_size > 1)
                for(size_t k = 0; k < delta_.size(); ++k)
                    delta_[k] = static_cast<ElemType>(delta_[k] / static_cast<double>(batch_size));
            Update();
        }
    }

    template <typename ElemType>
    inline void Model<ElemType>::Update()
    {
        for(size_t i = 0; i < weight_.size(); ++i)
        {
            weight_[i] -= alpha_ * delta_[i];
        }
    }
}