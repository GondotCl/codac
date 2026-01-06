#include "codac2_CtcQInter.h"

namespace codac2
{

    class CtcQInter_clara : public CtcQInter
    {

    public:
        using CtcQInter::CtcQInter;

        void set_q(size_t q)
        {
            assert_release(q <= nb());
            _q = q;
        }

        template <typename... C>
            requires(IsCtcBaseOrPtr<C, IntervalVector> && ...)
        void set_ctcs(const C &...c)
        {
            _ctcs = {c...};
        }

        template<typename C>
            requires IsCtcBaseOrPtr<C,IntervalVector>
        CtcQInter& operator&=(const C& c)
        {
            assert_release(size_of(c) == this->size());
            _ctcs.push_back(c);
            return *this;
        }
    };

}