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
        virtual ElemType AddTerm(std::vector<ElemType> *model);
    };

    template <typename ElemType>
    Regular<ElemType>::Regular(const Configure &config)
    {
        
    }

}
