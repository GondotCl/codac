#include <utility>
#include <memory>

template<typename T, typename... X>
class A
{
public:

    virtual ~A() = default;
    
    virtual void f(X... x) const = 0;

    virtual std::shared_ptr<A<X...>> copy() const
    {
        return std::make_shared<T>(*static_cast<const T*>(this));
    }
};

template<int N, typename X, typename = std::make_index_sequence<N + 1>>
class B;

template<int N, typename X, std::size_t... Is>
class B<N, X, std::index_sequence<Is...>> : public A<B<N, X>, Args...>
{

public:

    B() : A<B<N, X>, Args...>(N + 1) {}

    void f(Args... x) const override 
    { 
        // my code here 
    }
};
