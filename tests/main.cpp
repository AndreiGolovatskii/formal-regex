#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "nfa.h"

TEST(Constructors, Simple) {
    TNFAutomaton nfa_a{};
    assert(nfa_a.GetFinish() == nfa_a.GetStart());
}
