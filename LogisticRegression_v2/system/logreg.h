#include "model.h"
#include "log.h"
#include "configure.h"

namespace LR
{
    template <typename ElemType>
    class LogReg
    {
    public:
        LogReg(std::string config_file);
        void Train();
        void SaveModel();
        void Predict();

    private:
        Model<ElemType> *model_;
        Configure config_;
    };

    template <typename ElemType>
    LogReg<ElemType>::LogReg(std::string config_file) : config_(config_file)
    {
        model_ = Model<ElemType>::Get(config_);
    }

    template <typename ElemType>
    void LogReg<ElemType>::Train()
    {
        SampleReader<ElemType> *reader = nullptr;
        int batch_size = config_.mini_batch_size * 5;
        Sample<ElemType> **batchsamples = new Sample<ElemType>[batch_size];
        int sample_count = 0;
        int epoch = config_.train_epoch;
        for (int i = 0; i < epoch; ++i)
        {
            reader = new SampleReader<ElemType>(config_.train_file, config_.read_buffer_size, config_.input_dimention);
            sample_count = 0;
            sample_count = reader->GetSample(batch_size, batchsamples);
            model_->Train(sample_count, batchsamples);
            delete reader;
        }
        delete batchsamples;
    }
    template <typename ElemType>
    void LogReg<ElemType>::SaveModel()
    {
    }

    template <typename ElemType>
    void LogReg<ElemType>::Predict()
    {
    }
}
