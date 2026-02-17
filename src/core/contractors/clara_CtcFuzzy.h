#include "codac2_CtcQInter.h"

namespace codac2
{
  class CtcFuzzy : public Ctc<CtcFuzzy,IntervalVector>
  {
    public:
      explicit CtcFuzzy(double alpha, Index n, const Collection<CtcBase<IntervalVector>>& ctcs = {})
      : Ctc<CtcFuzzy,IntervalVector>(n), _alpha(alpha), _ctcs(ctcs)
      {
        assert_release(n > 0);
      }

      template<typename C>
        requires (IsCtcBaseOrPtr<C,IntervalVector> && !std::is_same_v<CtcFuzzy,C>)
      CtcFuzzy(double alpha, const C& c)
        : CtcFuzzy(alpha, size_of(c), {c})
      {}

      template<typename... C>
        requires (IsCtcBaseOrPtr<C,IntervalVector> && ...)
      CtcFuzzy(double alpha, const C&... c)
        : CtcFuzzy(alpha, size_first_item(c...), {c...})
      {
        assert_release(all_same_size(c...));
      }

      size_t nb() const
      {
        return _ctcs.size();
      }

      void contract(IntervalVector& x) const
      {
        if(_alpha <= 0)
          return;
      
        std::list<IntervalVector> l;

        for(const auto& ci : _ctcs)
        {
          IntervalVector x_(x);
          ci->contract(x_);
          l.push_back(x_);
        }

        x = fuzzyinter(_alpha, l, _marks);
      }

      // additionnal setters / operators

      void set_alpha(double alpha)
      {
        _alpha = alpha;
      }

      void set_marks(std::vector<double> m)
      {
        _marks = m;
      }

      template <typename... C>
        requires(IsCtcBaseOrPtr<C, IntervalVector> && ...)
      void set_ctcs(const C &...c)
      {
        _ctcs = {c...};
      }

      template<typename C>
        requires IsCtcBaseOrPtr<C,IntervalVector>
      CtcFuzzy& operator&=(const C& c)
      {
        assert_release(size_of(c) == this->size());
        _ctcs.push_back(c);
        return *this;
      }

    protected:
  
      double _alpha;
      Collection<CtcBase<IntervalVector>> _ctcs;
      std::vector<double> _marks = {};  
  };

}