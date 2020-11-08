#ifndef PRACTICUM_RPN_H
#define PRACTICUM_RPN_H

#include <cstdlib>
#include <stack>
#include <stdexcept>


template<class ValueType>
class TFunction {
public:
    virtual size_t GetArgCnt() const = 0;
    virtual ValueType operator()() const {
        throw std::logic_error("Invalid arguments number");
    }

    virtual ValueType operator()(const ValueType &first) const {
        (void)first;
        throw std::logic_error("Invalid arguments number");
    }

    virtual ValueType operator()(const ValueType &first, const ValueType &second) const {
        (void)first;
        (void)second;
        throw std::logic_error("Invalid arguments number");
    }
    virtual ~TFunction() {}
};

class TArithmeticalTokenNum: public TFunction<int> {
    int Value_;

public:
    virtual size_t GetArgCnt() const {
            return 0;
    };
    explicit TArithmeticalTokenNum(int value): Value_(value) {}
    int operator()() const final {
        return Value_;
    }
};

class TArithmeticalTokenPlus: public TFunction<int> {
public:
    virtual size_t GetArgCnt() const {
        return 2;
    }

    int operator()(const int& first, const int& second) const final {
        return first + second;
    }
};

class TArithmeticalTokenMinus: public TFunction<int> {
public:
    virtual size_t GetArgCnt() const {
        return 2;
    }

    int operator()(const int& first, const int& second) const final {
        return first - second;
    }
};

class TArithmeticalTokenUnaryMinus: public TFunction<int> {
public:
    virtual size_t GetArgCnt() const {
        return 1;
    }

    int operator()(const int& first) const final {
        return -first;
    }
};

template<class ValueType, class TokenContainer, class StackContainer = std::stack<ValueType>>
ValueType CalculateValue(const TokenContainer &tokens) {
    StackContainer stack;
    for (const auto &token : tokens) {
        if (token->GetArgCnt() == 0) {
            stack.push((*token)());
        } else if (token->GetArgCnt() == 1) {
            ValueType first = stack.top();
            stack.pop();
            stack.push((*token)(first));
        } else if (token->GetArgCnt() == 2) {
            ValueType second = stack.top();
            stack.pop();
            ValueType first = stack.top();
            stack.pop();

            stack.push((*token)(first, second));
        }
    }
    return stack.top();
}

#endif//PRACTICUM_RPN_H
