#include <gtest/gtest.h>
#include <vector>

#include "rpn.h"


class RPNTest: public ::testing::Test
{
protected:

    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {
    }
};


TEST_F(RPNTest, TestTFuntion) {
    TArithmeticalTokenNum num_1(16);
    TArithmeticalTokenNum num_2(12);
    TArithmeticalTokenPlus plus;
    ASSERT_EQ(plus(num_1(), num_2()), 28);

    EXPECT_THROW(((TFunction<int>&)plus)(2), std::logic_error);
    EXPECT_THROW(((TFunction<int>&)plus)(), std::logic_error);
    EXPECT_THROW(((TFunction<int>&)num_1)(2, 3), std::logic_error);
}

TEST_F(RPNTest, TestCalculate) {
    std::vector<std::unique_ptr<TFunction<int>>> v;
    v.emplace_back(new TArithmeticalTokenNum(15));
    v.emplace_back(new TArithmeticalTokenNum(16));
    v.emplace_back(new TArithmeticalTokenPlus());

    ASSERT_EQ(CalculateValue<int>(v), 31);

    v.emplace_back(new TArithmeticalTokenNum(10));
    v.emplace_back(new TArithmeticalTokenMinus());

    ASSERT_EQ(CalculateValue<int>(v), 21);

    v.emplace_back(new TArithmeticalTokenUnaryMinus());
    ASSERT_EQ(CalculateValue<int>(v), -21);

    v.emplace_back(new TArithmeticalTokenMinus);  // place 2-argument minus, only one argument provide
}
