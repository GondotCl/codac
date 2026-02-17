#define BOOST_TEST_MODULE DummyTests
#include <boost/test/included/unit_test.hpp>
#include <codac>
#include <iostream>

using namespace codac2;

BOOST_AUTO_TEST_CASE( constructor )
{
    CtcIdentity identity(2);
    CtcFuzzy(0., identity);
}

BOOST_AUTO_TEST_CASE( empty_ctcs )
{
    CtcIdentity identity(2);
    CtcFuzzy ctc(0., identity);
    ctc &= identity;
    ctc.set_ctcs();
}

struct Domains {
    IntervalVector res, x, y, z;
    std::vector<double> marks = {1., 3., 10.};
    Domains() : res(2), x({{1,4},{1,2}}), y({{5,6},{0,3}}), z({{3,7},{1.5,4}}) {}
};

template<typename First, typename... X>
auto create_wrapper(const First &first, const X &...x)
{
    Collection<CtcBase<First>> ctcs{CtcWrapper<First>(first)};
    (ctcs.push_back(CtcWrapper<First>(x)), ...);
    return ctcs;
}

BOOST_FIXTURE_TEST_SUITE( ctc_fuzzy_test, Domains )

BOOST_AUTO_TEST_CASE( test_all )
{
    CtcFuzzy ctc(14., 2, create_wrapper(x, y, z));
    ctc.set_marks(marks);
    ctc.contract(res);
    BOOST_CHECK_EQUAL(res, IntervalVector::empty(2));
}

BOOST_AUTO_TEST_CASE( test_xz )
{
    CtcFuzzy ctc(11., 2, create_wrapper(x, y, z));
    ctc.set_marks(marks);
    ctc.contract(res);
    BOOST_CHECK_EQUAL(res, IntervalVector({{3,6},{1.5,3}}));
}

BOOST_AUTO_TEST_CASE( test_xy )
{
    CtcFuzzy ctc(4., 2, create_wrapper(x, y, z));
    ctc.set_marks(marks);
    ctc.contract(res);
    BOOST_CHECK_EQUAL(res, IntervalVector({{3,7},{1,4}}));
}

BOOST_AUTO_TEST_CASE( test_yz )
{
    CtcFuzzy ctc(13, 2, create_wrapper(x, y, z));
    ctc.set_marks(marks);
    ctc.contract(res);
    BOOST_CHECK_EQUAL(res, IntervalVector({{5,6},{1.5,3}}));
}

BOOST_AUTO_TEST_CASE( test_none )
{
    CtcFuzzy ctc(0, 2, create_wrapper(x, y, z));
    ctc.set_marks(marks);
    ctc.contract(res);
    BOOST_CHECK_EQUAL(res, IntervalVector(2));
}

BOOST_AUTO_TEST_SUITE_END()
