#include "regular.h"
#include "math.h"

namespace LR
{
    template <typename ElemType>
    class L2Regular:public Regular<ElemType>
    {
    public:
        L2Regular(const Configure& config);
        virtual ~L2Regular() = default;
        virtual ElemType AddTerm(size_t idx, std::vector<ElemType> *model);
    };

    template <typename ElemType>
    L2Regular<ElemType>::L2Regular(const Configure& config):Regular<ElemType>(config)
    {
    }


    template <typename ElemType>
    ElemType L2Regular<ElemType>::AddTerm(size_t idx, std::vector<ElemType>* model)
    {
        return (ElemType)((*model)[idx] > 0 ? coef_ : -coef_);
    }   
}

