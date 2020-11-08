#include <gtest/gtest.h>


#include "solution.h"

TEST(TESTSolution, Sample) {
    ASSERT_EQ(Solve("ab+c.aba.*.bac.+.+*", 3, 2), true);
    ASSERT_EQ(Solve("acb..bab.c.*.ab.ba.+.+*a.", 3, 0), false);
}