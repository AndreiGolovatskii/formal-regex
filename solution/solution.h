#ifndef PRACTICUM_SOLUTION_H
#define PRACTICUM_SOLUTION_H


#include "nfa.h"

class TTaskSolveVisitor {
    std::vector<std::vector<char>> Visited_;
    const int Finish_;
public:
    const int Mod;
    int CurrentLenMod = 0;
    TTaskSolveVisitor(const TNFAutomaton& nfa, int mod)
            : Visited_(nfa.VertexCount(), std::vector<char>(mod, false)),
              Finish_(static_cast<int>(nfa.GetFinish())), Mod(mod) {}

    void ProcessVertex(TNFAVertex vertex) { Visited_[static_cast<int>(vertex)][CurrentLenMod] = true; }
    bool ProcessEdge(TNFAEdge edge) {
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
    void ReturnByEdge(TNFAEdge edge) {
        if (edge.GetC() != TNFAutomaton::EPS) {
            CurrentLenMod = (CurrentLenMod - 1 + Mod) % Mod;
        }
    }
    bool HasSolution(int len) const { return Visited_[Finish_][len]; }
};


bool Solve(const TNFAutomaton& nfa, int mod, int len) {
    TTaskSolveVisitor solver(nfa, mod);
    nfa.VisitDFS(solver);
}

#endif//PRACTICUM_SOLUTION_H
