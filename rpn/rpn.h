#ifndef PRACTICUM_RPN_H
#define PRACTICUM_RPN_H

#include <cstdlib>
#include <stack>
#include <stdexcept>


template<class ValueType>
class TVAFunction {
public:
    [[nodiscard]] virtual size_t GetArgCnt() const = 0;
    virtual ValueType operator()() const { throw std::logic_error("Invalid arguments number"); }

    virtual ValueType operator()(const ValueType& first) const {
        (void) first;
        throw std::logic_error("Invalid arguments number");
    }

    virtual ValueType operator()(const ValueType& first, const ValueType& second) const {
        (void) first;
        (void) second;
        throw std::logic_error("Invalid arguments number");
    }
    virtual ~TVAFunction() {}
};


class TArithmeticalTokenNum : public TVAFunction<int> {
    int Value_;

public:
    explicit TArithmeticalTokenNum(int value) : Value_(value) {}

    [[nodiscard]] size_t GetArgCnt() const final { return 0; };

    int operator()() const final { return Value_; }
};


class TArithmeticalTokenPlus : public TVAFunction<int> {
public:
    [[nodiscard]] size_t GetArgCnt() const final { return 2; }

    int operator()(const int& first, const int& second) const final { return first + second; }
};


class TArithmeticalTokenMinus : public TVAFunction<int> {
public:
    [[nodiscard]] size_t GetArgCnt() const final { return 2; }

    int operator()(const int& first, const int& second) const final { return first - second; }
};


class TArithmeticalTokenUnaryMinus : public TVAFunction<int> {
public:
    [[nodiscard]] size_t GetArgCnt() const final { return 1; }

    int operator()(const int& first) const final { return -first; }
};


template<class ValueType, class TokenPtrContainer, class StackContainer = std::stack<ValueType>>
ValueType CalculateRPNExpression(const TokenPtrContainer& tokenPtrs) {
    StackContainer stack;
    for (const auto& token : tokenPtrs) {
        if (token->GetArgCnt() == 0) {
            stack.push((*token)());
        } else if (token->GetArgCnt() == 1) {
            if (stack.size() < 1) {
                throw std::logic_error("Incorrect expression");
            }
            ValueType first = stack.top();
            stack.pop();
            stack.push((*token)(first));
        } else if (token->GetArgCnt() == 2) {
            if (stack.size() < 2) {
                throw std::logic_error("Incorrect expression");
            }
            ValueType second = stack.top();
            stack.pop();
            ValueType first = stack.top();
            stack.pop();

            stack.push((*token)(first, second));
        }
    }
    if (stack.size() != 1) { throw std::logic_error("Incorrect expression, stack not empty at end"); }
    return stack.top();
}

#endif//PRACTICUM_RPN_H
