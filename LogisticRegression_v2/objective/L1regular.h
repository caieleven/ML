#include "regular.h"
#include "math.h"

namespace LR
{
    template <typename ElemType>
    class L1Regular:public Regular<ElemType>
    {
    public:
        L1Regular(const Configure& config);
        virtual ~L1Regular() = default;
        virtual ElemType AddTerm(size_t idx, std::vector<ElemType> *model);
    };

    template <typename ElemType>
    L1Regular<ElemType>::L1Regular(const Configure& config):Regular<ElemType>(config)
    {
    }


    template <typename ElemType>
    ElemType L1Regular<ElemType>::AddTerm(size_t idx, std::vector<ElemType>* model)
    {
        return (ElemType)((*model)[idx] > 0 ? coef_ : -coef_);
    }   
}

