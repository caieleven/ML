#include "configure.h"
#include <vector>

namespace LR
{
    template <typename ElemType>
    class Regular
    {
    public:
        Regular(const Configure &config);
        static Get(const Configure &config);
        virtual ~Regular() = default;
        virtual ElemType AddTerm(size_t idx, std::vector<ElemType> *model);

    private:
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
        virtual ElemType AddTerm(size_t idx, std::vector<ElemType> *model);
    };

    template <typename ElemType>
    class L2Regular : public Regular<ElemType>
    {
    public:
        L2Regular(const Configure &config);
        virtual ~L2Regular() = default;
        virtual ElemType AddTerm(size_t idx, std::vector<ElemType> *model);
    };

    template <typename ElemType>
    Regular<ElemType>::Regular(const Configure &config) : coef_(config.regular_coef),
                                                          input_dimention_(config.input_dimention),
                                                          output_dimention_(config.output_dimention)
    {
    }

    template <typename ElemType>
    L1Regular<ElemType>::L1Regular(const Configure &config) : Regular<ElemType>(config)
    {
    }

    template <typename ElemType>
    ElemType L1Regular<ElemType>::AddTerm(size_t idx, std::vector<ElemType> *model)
    {
        return (ElemType)((*model)[idx] > 0 ? coef_ : -coef_);
    }

    template <typename ElemType>
    L2Regular<ElemType>::L2Regular(const Configure &config) : Regular<ElemType>(config)
    {
    }

    template <typename ElemType>
    ElemType L2Regular<ElemType>::AddTerm(size_t idx, std::vector<ElemType> *model)
    {
        return (ElemType)(abs((*model)[idx]) * coef_);
    }
    //template <typename ElemType>

}
