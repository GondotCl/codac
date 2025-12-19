#define BOOST_TEST_MODULE DummyTests
#include <boost/test/included/unit_test.hpp>
#include <codac>
#include <iostream>

using namespace codac2;

struct Domains {
    IntervalVector res, x, y, z;
    Domains() : res(2), x({{1,4},{1,2}}), y({{5,6},{0,3}}), z({{3,7},{1.5,4}}) {}
};

BOOST_FIXTURE_TEST_SUITE( ctc_relaxed_test, Domains )

BOOST_AUTO_TEST_CASE( test_0 )
{
    CtcRelaxed<3, IntervalVector> ctc(2);
    ctc.set_q(0);
    ctc.contract(res, x, y, z);
    BOOST_CHECK_EQUAL(res, IntervalVector::empty(2));
}

BOOST_AUTO_TEST_CASE( test_1 )
{
    CtcRelaxed<3, IntervalVector> ctc(2);
    ctc.set_q(1);
    ctc.contract(res, x, y, z);
    BOOST_CHECK_EQUAL(res, IntervalVector({{3,6},{1,3}}));
}

BOOST_AUTO_TEST_CASE( test_2 )
{
    CtcRelaxed<3, IntervalVector> ctc(2);
    ctc.set_q(2);
    ctc.contract(res, x, y, z);
    BOOST_CHECK_EQUAL(res, IntervalVector({{1,7},{0,4}}));
}

BOOST_AUTO_TEST_CASE( test_3 )
{
    CtcRelaxed<3, IntervalVector> ctc(2);
    ctc.set_q(3);
    ctc.contract(res, x, y, z);
    BOOST_CHECK_EQUAL(res, IntervalVector(2));
}

BOOST_AUTO_TEST_SUITE_END()
