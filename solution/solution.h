#ifndef PRACTICUM_SOLUTION_H
#define PRACTICUM_SOLUTION_H

#include <vector>
#include <memory>

#include "nfa.h"
#include "rpn.h"


class TTaskSolveVisitor {
    std::vector<std::vector<char>> Visited_;
    const int Finish_;

public:
    const int Mod;
    int CurrentLenMod = 0;
    TTaskSolveVisitor(const TNFAutomaton& nfa, int mod)
        : Visited_(nfa.VertexCount(), std::vector<char>(mod, false)), Finish_(nfa.GetFinish()),
          Mod(mod) {}

    void ProcessVertex(TConstNFAVertex vertex) { Visited_[vertex][CurrentLenMod] = true; }
    bool ProcessEdge(TConstNFAEdge edge) {
        int nextLenMod;
        if (edge.GetC() == TNFAutomaton::EPS) {
            nextLenMod = CurrentLenMod;
        } else {
            nextLenMod = (CurrentLenMod + 1) % Mod;
        }
        if (!Visited_[edge.GetTo()][nextLenMod]) {
            CurrentLenMod = nextLenMod;
            return true;
        }
        return false;
    }
    void ReturnByEdge(TConstNFAEdge edge) {
        if (edge.GetC() != TNFAutomaton::EPS) { CurrentLenMod = (CurrentLenMod - 1 + Mod) % Mod; }
    }
    [[nodiscard]] bool HasSolution(int len) const { return Visited_[Finish_][len]; }
};


using TNFAToken = TVAFunction<TNFAutomaton>;

class TNFATokenPlus : public TNFAToken {
public:
    [[nodiscard]] size_t GetArgCnt() const final { return 2; }
    TNFAutomaton operator()(const TNFAutomaton& a, const TNFAutomaton& b) const final { return a + b; }
};

class TNFATokenMul : public TNFAToken {
public:
    [[nodiscard]] size_t GetArgCnt() const final { return 2; }
    TNFAutomaton operator()(const TNFAutomaton& a, const TNFAutomaton& b) const final { return a * b; }
};

class TNFATokenKleeneStar : public TNFAToken {
public:
    [[nodiscard]] size_t GetArgCnt() const final { return 1; }
    TNFAutomaton operator()(const TNFAutomaton& a) const final { return a.KleeneStar(); }
};

class TNFATokenNFA : public TNFAToken {
    TNFAutomaton Value_;

public:
    explicit TNFATokenNFA(const TNFAutomaton& value) : Value_(value) {}
    explicit TNFATokenNFA(TNFAutomaton&& value) : Value_(std::move(value)) {}
    [[nodiscard]] size_t GetArgCnt() const final { return 0; }
    TNFAutomaton operator()() const final { return Value_; }
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
        } else if (isalpha(c)) {
            res.emplace_back(new TNFATokenNFA{TNFAutomaton{c}});
        } else if (c == '1') {
            res.emplace_back(new TNFATokenNFA{TNFAutomaton{}});
        } else {
            throw std::logic_error("Unexpected symbol");
        }
    }
    return res;
}

bool Solve(const std::string& regexp, int mod, int len) {
    auto tokens = ParseRPNString(regexp);
    auto nfa = CalculateRPNExpression<TNFAutomaton>(tokens);

    TTaskSolveVisitor solver(nfa, mod);
    nfa.VisitDFS(solver);
    return solver.HasSolution(len);
}

#endif//PRACTICUM_SOLUTION_H
