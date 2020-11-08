#include "nfa.h"

void TNFAVertex::AddEdge(const TNFAVertex& other, char c) { Nfa_.AddEdge_(Id_, other.Id_, c); }


TNFAutomaton operator*(const TNFAutomaton& first, const TNFAutomaton& second) {
    TNFAutomaton res = first;
    res *= second;
    return res;
}

TNFAutomaton operator+(const TNFAutomaton& first, const TNFAutomaton& second) {
    TNFAutomaton res = first;
    res += second;
    return res;
}