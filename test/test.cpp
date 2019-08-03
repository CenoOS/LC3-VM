//
// Created by XingfengYang on 2019-08-04.
//

#define BOOST_TEST_MODULE SqrTests

#include <boost/test/unit_test.hpp>

#include "sqr.h"

BOOST_AUTO_TEST_CASE(FailTest){
        BOOST_CHECK_EQUAL(5, sqr(2));
}

BOOST_AUTO_TEST_CASE(PassTest){
        BOOST_CHECK_EQUAL(4, sqr(2));
}