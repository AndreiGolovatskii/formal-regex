#ifndef PRACTICUM_SOLUTION_H
#define PRACTICUM_SOLUTION_H

#include <memory>
#include <string>
#include <unordered_map>

#include "nfa.h"
#include "rpn.h"


class TTaskSolveVisitor {
    std::unordered_map<const TNFAVertex*, std::vector<char>> Visited_;
    const TNFAVertex* Finish_;

public:
    const size_t Mod;
    size_t CurrentLenMod = 0;
    TTaskSolveVisitor(const TNFAutomaton& nfa, size_t mod) : Finish_(nfa.GetFinish()), Mod(mod) {}

    inline void ProcessVertex(const TNFAVertex* vertex) {
        if (Visited_[vertex].empty()) { Visited_[vertex].resize(Mod, false); }
        Visited_[vertex][CurrentLenMod] = true;
    }
    bool ProcessEdge(TNFAEdge edge) {
        size_t nextLenMod;
        if (edge.GetC() == TNFAutomaton::EPS) {
            nextLenMod = CurrentLenMod;
        } else {
            nextLenMod = (CurrentLenMod + 1) % Mod;
        }
        if (Visited_[edge.GetTo()].size() != Mod || !Visited_[edge.GetTo()][nextLenMod]) {
            CurrentLenMod = nextLenMod;
            return true;
        }
        return false;
    }
    inline void ReturnByEdge(TNFAEdge edge) {
        if (edge.GetC() != TNFAutomaton::EPS) { CurrentLenMod = (CurrentLenMod - 1 + Mod) % Mod; }
    }
    [[nodiscard]] bool HasSolution(size_t len) const {
        return Visited_.find(Finish_) != Visited_.end() && Visited_.at(Finish_).size() == Mod &&
               Visited_.at(Finish_)[len];
    }
};


using TNFAToken = TVAFunction<TNFAutomaton>;

class TNFATokenPlus : public TNFAToken {
public:
    [[nodiscard]] size_t GetArgCnt() const final { return 2; }
    TNFAutomaton operator()(TNFAutomaton&& a, TNFAutomaton&& b) final { return std::move(a) + std::move(b); }
};

class TNFATokenMul : public TNFAToken {
public:
    [[nodiscard]] size_t GetArgCnt() const final { return 2; }
    TNFAutomaton operator()(TNFAutomaton&& a, TNFAutomaton&& b) final { return std::move(a) * std::move(b); }
};

class TNFATokenKleeneStar : public TNFAToken {
public:
    [[nodiscard]] size_t GetArgCnt() const final { return 1; }
    TNFAutomaton operator()(TNFAutomaton&& a) final { return std::move(a.KleeneStarInplace()); }
};

class TNFATokenNFA : public TNFAToken {
    TNFAutomaton Value_;

public:
    explicit TNFATokenNFA(TNFAutomaton&& value) : Value_(std::move(value)) {}
    [[nodiscard]] size_t GetArgCnt() const final { return 0; }
    TNFAutomaton operator()() final { return std::move(Value_); }
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
            res.emplace_back(new TNFATokenNFA{TNFAutomaton{TNFAutomaton::EPS}});
        } else if (c == ' ') {
            continue;
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
