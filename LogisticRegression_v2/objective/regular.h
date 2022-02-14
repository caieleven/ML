#include "configure.h"
#include <math.h>
#include <vector>

namespace LR
{
    template <typename ElemType>
    class Regular
    {
    public:
        Regular(const Configure &config);
        static Regular<ElemType> *Get(const Configure &config);
        //virtual ~Regular();
        inline virtual ElemType AddTerm(size_t idx, std::vector<ElemType> &model);

    protected:
        float coef_;
        int input_dimention_;
        int output_dimention_;
    };

    template <typename ElemType>
    class L1Regular : public Regular<ElemType>
    {
    public:
        L1Regular(const Configure &config);
        virtual ~L1Regular() = default;
        inline virtual ElemType AddTerm(size_t idx, std::vector<ElemType> &model);
    };

    template <typename ElemType>
    class L2Regular : public Regular<ElemType>
    {
    public:
        L2Regular(const Configure &config);
        virtual ~L2Regular() = default;
        inline virtual ElemType AddTerm(size_t idx, std::vector<ElemType> &model);
    };

    template <typename ElemType>
    Regular<ElemType>::Regular(const Configure &config) : coef_(config.regular_coef),
                                                          input_dimention_(config.input_dimention),
                                                          output_dimention_(config.output_dimention)
    {
    }

    template <typename ElemType>
    Regular<ElemType> *Regular<ElemType>::Get(const Configure &config)
    {
        if (config.regularizer == "L1")
            return new L1Regular<ElemType>(config);
        else if (config.regularizer == "L2")
            return new L2Regular<ElemType>(config);
        else
            return new Regular<ElemType>(config);
    }

    template <typename ElemType>
    inline ElemType Regular<ElemType>::AddTerm(size_t idx, std::vector<ElemType> &model)
    {
        return 0;
    }

    template <typename ElemType>
    L1Regular<ElemType>::L1Regular(const Configure &config) : Regular<ElemType>(config)
    {
    }

    template <typename ElemType>
    inline ElemType L1Regular<ElemType>::AddTerm(size_t idx, std::vector<ElemType> &model)
    {
        return (ElemType)(model[idx] > 0 ? this->coef_ : - this->coef_);
    }

    template <typename ElemType>
    L2Regular<ElemType>::L2Regular(const Configure &config) : Regular<ElemType>(config)
    {
    }

    template <typename ElemType>
    inline ElemType L2Regular<ElemType>::AddTerm(size_t idx, std::vector<ElemType> &model)
    {
        return (ElemType)(abs(model[idx]) * this->coef_);
    }
    // template <typename ElemType>

}
