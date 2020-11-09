#include <gtest/gtest.h>

#include "solution.h"


TEST(TESTSolution, Sample) {
    ASSERT_TRUE(Solve("ab+c.aba.*.bac.+.+*", 3, 2));
    ASSERT_FALSE(Solve("acb..bab.c.*.ab.ba.+.+*a.", 3, 0));
}


TEST(TESTSolution, WithEPS) {
    ASSERT_TRUE(Solve("1", 10, 0));
    ASSERT_FALSE(Solve("1", 10, 1));

    ASSERT_TRUE(Solve("1*", 10, 0));
    ASSERT_FALSE(Solve("1*", 10, 1));

    ASSERT_TRUE(Solve("1a*+", 10, 0));
    ASSERT_TRUE(Solve("1a*+", 10, 1));
    ASSERT_TRUE(Solve("11.", 10, 0));
}


TEST(TESTSolution, Simple) {
    ASSERT_FALSE(Solve("ab.*", 2, 1));  // (ab)*
    ASSERT_FALSE(Solve("1abc+..*", 2, 1));  // (1.a.(b+c))*
    ASSERT_TRUE(Solve("1abc+..*", 2, 0));  // (1.a.(b+c))*

    ASSERT_FALSE(Solve("abc..*d+", 3, 2)); // (abc)*+d
    ASSERT_TRUE(Solve("abc..*d+", 3, 1)); // (abc)*+d
}


TEST(TESTSolution, IncorrectExprs) {
    EXPECT_THROW(Solve("aa++", 1, 0), std::logic_error);
    EXPECT_THROW(Solve("aaa+", 1, 0), std::logic_error);
    EXPECT_THROW(Solve("+", 1, 0), std::logic_error);
    EXPECT_THROW(Solve("", 1, 0), std::logic_error);
    EXPECT_THROW(Solve("777", 1, 0), std::logic_error);
}