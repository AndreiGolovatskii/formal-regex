#ifndef PRACTICUM_SOLUTION_H
#define PRACTICUM_SOLUTION_H

#include <vector>

#include "nfa.h"
#include "rpn.h"

class TTaskSolveVisitor {
    std::vector<std::vector<char>> Visited_;
    const int Finish_;

public:
    const int Mod;
    int CurrentLenMod = 0;
    TTaskSolveVisitor(const TNFAutomaton &nfa, int mod)
        : Visited_(nfa.VertexCount(), std::vector<char>(mod, false)), Finish_(static_cast<int>(nfa.GetFinish())),
          Mod(mod) {}

    void ProcessVertex(TConstNFAVertex vertex) { Visited_[static_cast<int>(vertex)][CurrentLenMod] = true; }
    bool ProcessEdge(TConstNFAEdge edge) {
        int nextLenMod;
        if (edge.GetC() == TNFAutomaton::EPS) {
            nextLenMod = CurrentLenMod;
        } else {
            nextLenMod = (CurrentLenMod + 1) % Mod;
        }
        if (!Visited_[static_cast<int>(edge.GetTo())][nextLenMod]) {
            CurrentLenMod = nextLenMod;
            return true;
        }
        return false;
    }
    void ReturnByEdge(TConstNFAEdge edge) {
        if (edge.GetC() != TNFAutomaton::EPS) { CurrentLenMod = (CurrentLenMod - 1 + Mod) % Mod; }
    }
    bool HasSolution(int len) const { return Visited_[Finish_][len]; }
};

class TNFATokenPlus: public TVAFunction<TNFAutomaton> {
public:
    size_t GetArgCnt() const final {
        return 2;
    }
    TNFAutomaton operator()(const TNFAutomaton& a, const TNFAutomaton& b) const final {
        return a + b;
    }
};


using TNFAToken = TVAFunction<TNFAutomaton>;

class TNFATokenMul: public TNFAToken {
public:
    size_t GetArgCnt() const final {
        return 2;
    }
    TNFAutomaton operator()(const TNFAutomaton& a, const TNFAutomaton& b) const final {
        return a * b;
    }
};

class TNFATokenKleeneStar: public TNFAToken {
public:
    size_t GetArgCnt() const final {
        return 1;
    }
    TNFAutomaton operator()(const TNFAutomaton& a) const final {
        return a.KleeneStar();
    }
};

class TNFATokenNFA: public TNFAToken {
    TNFAutomaton Value_;
public:
    explicit TNFATokenNFA(const TNFAutomaton& value): Value_(value) {}
    size_t GetArgCnt() const final {
        return 0;
    }
    TNFAutomaton operator()() const final {
        return Value_;
    }
};

std::vector<std::unique_ptr<TNFAToken>> ParseRPNString(const std::string& s) {
    std::vector<std::unique_ptr<TNFAToken>> res;
    for (const auto& c : s) {
        if (c == '.') {
            res.emplace_back(new TNFATokenMul{});
        } else if (c == '*') {
            res.emplace_back(new TNFATokenKleeneStar{});
        } else if (c == '+') {
            res.emplace_back(new TNFATokenPlus{});
        } else {
            res.emplace_back(new TNFATokenNFA{TNFAutomaton{c}});
        }
    }
    return res;
}

bool Solve(const std::string& regexp, int mod, int len) {
    auto tokens = ParseRPNString(regexp);
    auto nfa = CalculateValue<TNFAutomaton>(tokens);

    TTaskSolveVisitor solver(nfa, mod);
    nfa.VisitDFS(solver);
    return solver.HasSolution(len);
}

#endif//PRACTICUM_SOLUTION_H
