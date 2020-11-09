#include "nfa.h"


void TNFAVertex::AddEdge(TNFAVertex* other, char c) {
    AdjEdges_.emplace_back(other, c);
}

TNFAutomaton::TNFAutomaton() {}


TNFAutomaton::TNFAutomaton(char c): TNFAutomaton() {
    auto newStartPtr = NewVertex();
    auto newFinishPtr = NewVertex();

    newStartPtr->AddEdge(newFinishPtr, c);
    SetStart(newStartPtr);
    SetFinish(newFinishPtr);
}


TNFAutomaton& TNFAutomaton::operator+=(TNFAutomaton&& other) {
    if (this == &other) {
        Clear();
        return *this;
    }
    VertexesListBottom_->Next = std::move(other.VertexesListTop_);
    VertexesListBottom_ = other.VertexesListBottom_;
    Start_->AddEdge(other.Start_, EPS);
    other.Finish_->AddEdge(Finish_, EPS);
    other.Clear();
    return *this;
}


TNFAutomaton operator+(TNFAutomaton&& first, TNFAutomaton&& second) {
    TNFAutomaton res = std::move(first);
    res += std::move(second);
    return res;
}


TNFAutomaton& TNFAutomaton::operator*=(TNFAutomaton&& other) {
    if (this == &other) {
        Clear();
        return *this;
    }
    VertexesListBottom_->Next = std::move(other.VertexesListTop_);
    VertexesListBottom_ = other.VertexesListBottom_;
    Finish_->AddEdge(other.Start_, EPS);
    Finish_ = other.Finish_;
    other.Clear();
    return *this;
}


TNFAutomaton operator*(TNFAutomaton&& first, TNFAutomaton&& second) {
    TNFAutomaton res = std::move(first);
    res *= std::move(second);
    return res;
}


TNFAutomaton& TNFAutomaton::KleeneStarInplace() {
    TNFAVertex* newFinish = NewVertex();
    Start_->AddEdge(newFinish, EPS);
    Finish_->AddEdge(Start_, EPS);
    Finish_ = newFinish;
    return *this;
}


void TNFAutomaton::Iterator::EpsClosure_() {
    std::queue<const TNFAVertex*> Vqueue;
    for (const auto* vertex : Vertexes_) { Vqueue.push(vertex); }
    while (!Vqueue.empty()) {
        const TNFAVertex* vertex = Vqueue.front();
        Vqueue.pop();
        for (const auto& edge : vertex->AdjEdges_) {
            if (edge.C == EPS && Vertexes_.count(edge.To) == 0) {
                Vqueue.push(edge.To);
                Vertexes_.insert(edge.To);
            }
        }
    }
}


void TNFAutomaton::Iterator::Next(char c) {
    std::unordered_set<const TNFAVertex*> newVertexes;
    for (const auto* vertex : Vertexes_) {
        for (const auto& edge : vertex->AdjEdges_) {
            if (edge.C == c) { newVertexes.insert(edge.To); }
        }
    }
    Vertexes_ = newVertexes;
    EpsClosure_();
}
