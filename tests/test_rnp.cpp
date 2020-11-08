#include <gtest/gtest.h>
#include <vector>

#include "rpn.h"


TEST(RPNTest, TestTFuntion) {
    TArithmeticalTokenNum num_1(16);
    TArithmeticalTokenNum num_2(12);
    TArithmeticalTokenPlus plus;
    ASSERT_EQ(plus(num_1(), num_2()), 28);

    EXPECT_THROW(((TVAFunction<int>&)plus)(2), std::logic_error);
    EXPECT_THROW(((TVAFunction<int>&)plus)(), std::logic_error);
    EXPECT_THROW(((TVAFunction<int>&)num_1)(2, 3), std::logic_error);
}


TEST(RPNTest, TestCalculate) {
    std::vector<std::unique_ptr<TVAFunction<int>>> expr;
    expr.emplace_back(new TArithmeticalTokenNum(15));
    ASSERT_EQ(CalculateValue<int>(expr), 15);
    expr.emplace_back(new TArithmeticalTokenNum(16));
    expr.emplace_back(new TArithmeticalTokenPlus());

    ASSERT_EQ(CalculateValue<int>(expr), 31);

    expr.emplace_back(new TArithmeticalTokenNum(10));
    expr.emplace_back(new TArithmeticalTokenMinus());

    ASSERT_EQ(CalculateValue<int>(expr), 21);

    expr.emplace_back(new TArithmeticalTokenUnaryMinus());
    ASSERT_EQ(CalculateValue<int>(expr), -21);

    expr.emplace_back(new TArithmeticalTokenMinus);  // place 2-argument minus, only one argument provide
}


TEST(RPNTest, TestIncorrectExpr) {
    std::vector<std::unique_ptr<TVAFunction<int>>> expr;
    expr.emplace_back(new TArithmeticalTokenNum(12));
    expr.emplace_back(new TArithmeticalTokenNum(13));
    expr.emplace_back(new TArithmeticalTokenNum(14));
    expr.emplace_back(new TArithmeticalTokenMinus());

    EXPECT_THROW(CalculateValue<int>(expr), std::logic_error);
}