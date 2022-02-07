#include "configure.h"
#include "io.h"
#include "datatype.h"
#include <vector>

namespace LR
{
    template <class ElemType>
    class Objective
    {
    public:
        Objective(Configure &config);
        static Get(const Configure& config);
        virtual float Gradient(Sample<ElemType> sample, std::vector<float> model);

        

    protected:
        bool Regularization_;
        int input_dimention_;
        int output_dimention_;


    };
}
