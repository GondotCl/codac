// Le contracteur Q Inter

#pragma once

#include <vector>

#include <utility>
#include <iostream>

#include "codac2_Ctc.h"

struct borne
{
    double val;
    int ouverture;
    borne(const double &a, const int &b) : val(a), ouverture(b) {}
    friend bool operator<(const borne &x, const borne &y) { return x.val < y.val; }
};

// Primary template
template<typename T, typename X, typename Seq>
struct repeat_type;

// Specialization using index_sequence
template<typename T, typename X, std::size_t... Is>
struct repeat_type<T, X, std::index_sequence<Is...>>
{
    // The `(void)Is, T` trick repeats T N times
    using type = codac2::Ctc<T, decltype((void)Is, X{})...>;
};

namespace codac2
{

    template <int N, typename X>
    class CtcRelaxed;

    template<int N, typename T, typename Seq>
    class CtcRelaxedImpl;


    template<int N, typename X, std::size_t... Is>
    class CtcRelaxedImpl<N, X, std::index_sequence<Is...>> : public Ctc<CtcRelaxedImpl<N, X, std::index_sequence<Is...>>, decltype((void)Is, X{})...>
    {
    public:
        CtcRelaxedImpl(const int n_dim)
            : Ctc<CtcRelaxedImpl<N, X, std::index_sequence<Is...>>, decltype((void)Is, X{})...>(n * (N + 1)), n(N), dim(n_dim) { assert(n_dim > 0); } // todo : get Is size

        void contract(decltype((void)Is, X{})&... domains) const override
        {
            bool res = true;
            assert((res &= ... &= (domains.size() == dim)));

            IntervalVector x(dim);

            if ((n - _q) <= 0)
            {
                _contract(x, domains...);
                return;
            }

            for (int i = 0; i < dim; i++)
            {
                std::vector<borne> v_bornes;
                to_bornes_except_first(v_bornes, i, domains...);
                std::sort(v_bornes.begin(), v_bornes.end());
                int sum = 0;
                int index = 0;
                int imin = n;
                int imax = -1;
                for (auto b : v_bornes)
                {
                    int o = b.ouverture;
                    sum += o;
                    if (o > 0)
                    {
                        if (sum >= n - _q)
                            imin = imin < index ? imin : index;
                    }
                    else if (o < 0)
                    {
                        if (sum >= n - _q - 1)
                            imax = imax > index ? imax : index;
                    }
                    index++;
                }
                Interval result = Interval::empty();
                if (imax > -1)
                    result |= Interval(v_bornes[imin].val, v_bornes[imax].val);
                x[i] &= result;
            }
            _contract(x, domains...);
        }
    
        void set_q(int i) { _q = i; }
    protected:
        const int n;
        const int dim;
        int _q;

        /**
         * @brief Converts an Interval to bornes
         */
        template <typename Domain>
            requires(std::is_same_v<Domain, Interval>)
        std::pair<borne, borne> bornes(const Domain &x, const int &i) const
        {
            return {{x.ub(), -1}, {x.lb(), 1}};
        }

        /**
         * @brief Converts an IntervalVector i-th dimension to bornes
         */
        template <typename Domain>
            requires(std::is_same_v<Domain, IntervalVector>)
        std::pair<borne, borne> bornes(const Domain &x, const int &i) const
        {
            return {{x[i].ub(), -1}, {x[i].lb(), 1}};
        }

        /**
         * @brief Takes the first given domain and intersects with x
         *
         * @param[in] x Result of the contraction
         * @param[out] res First given domain
         * @param domains The rest of the given domains
         */
        template <typename Result, typename... Ignored>
        void _contract(const IntervalVector &x, Result &res, Ignored &...domains) const { res = res & x; }

        /**
         * @brief Pushes bornes in the given vector
         *
         * @param[out] v Container
         * @param[in] i Current dimension
         * @param[in] x Input domain
         */
        template <typename Domain>
        void to_bornes(std::vector<borne> &v, const int &i, const Domain &x) const
        {
            auto [borne1, borne2] = bornes(x, i);
            v.push_back(borne1);
            v.push_back(borne2);
        }

        /**
         * @brief Pushes bornes in the given vector recursively
         *
         * @param[out] v Container
         * @param[in] i Current dimension
         * @param[in] x Input domain
         * @param[in] y Next input domain
         * @param[in] domains The rest of the given domains
         */
        template <typename First, typename Second, typename... Rest>
        void to_bornes(std::vector<borne> &v, const int &i, const First &x, const Second &y, const Rest &...domains) const
        {
            to_bornes(v, i, x);
            to_bornes(v, i, y, domains...);
        }

        /**
         * @brief Pushes bornes in the given vector but ignores the first one
         *
         * @param[out] v Container
         * @param[in] i Current dimension
         * @param[in] first Ignored domain
         * @param[in] rest Input domains
         */
        template <typename Ignored, typename... Rest>
        void to_bornes_except_first(std::vector<borne> &v, const int &i, const Ignored &first, const Rest &...rest) const
        {
            to_bornes(v, i, rest...);
        }
    };

    /**
     * @brief Contractor computing the relaxed intersection
     * 
     * See https://en.wikipedia.org/wiki/Relaxed_intersection
     * 
     * @tparam Domains To be contracted types
     */
    template <int N, typename X>
    class CtcRelaxed : public CtcRelaxedImpl<N, X, std::make_index_sequence<N + 1>>
    {
    public:
        /**
         * @brief Construct a new Ctc Relaxed object
         * 
         * @param n_dim Number of dimensions of the contracted domains
         */
        using CtcRelaxedImpl<N, X, std::make_index_sequence<N + 1>>::CtcRelaxedImpl;

        /**
         * @brief Set the maximum authorized number of outliers
         *
         * @param[in] i
         */
        using CtcRelaxedImpl<N, X, std::make_index_sequence<N + 1>>::set_q;

        /**
         * @brief Main contraction algorithms
         *
         * The first given domain is used to contain the result
         * of the q-relaxed intersion of all the other domains.
         *
         * @param domains
         */
        using CtcRelaxedImpl<N, X, std::make_index_sequence<N + 1>>::contract;
        // template<typename... Domains>
        // void contract(Domains &...domains) const
            
        // const int dim; /** Number of dimensions of the contracted domains */
        // const int n;   /** Number of given domains, excluding the one used as container */
        // int _q;        /** Maximum authorized number of outliers */
    };

}
