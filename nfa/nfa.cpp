#include "nfa.h"

TConstNFAVertex::TConstNFAVertex(const TNFAutomaton& nfa, int id) : Nfa_(const_cast<TNFAutomaton&>(nfa)), Id_(id) {}


void TNFAVertex::AddEdge(const TNFAVertex& other, char c) { Nfa_.AddEdge_(Id_, other.Id_, c); }


TConstNFAEdge::TConstNFAEdge(const TNFAutomaton& nfa, int from, int to, char c)
    : Nfa_(const_cast<TNFAutomaton&>(nfa)), From_(from), To_(to), C_(c) {}


TNFAEdge::TNFAEdge(TNFAutomaton& nfa, int from, int to, char c) : TConstNFAEdge(nfa, from, to, c) {}


TNFAutomaton::TNFAutomaton() : Start_(0), Finish_(0), EdgesFrom_(1) {}


TNFAutomaton::TNFAutomaton(char c) : Start_(0), Finish_(1), EdgesFrom_(2) { EdgesFrom_[Start_].emplace_back(Finish_, c); }


TNFAutomaton& TNFAutomaton::operator+=(const TNFAutomaton& other) {
    int align = ExtendBy_(other);
    AddEdge_(Start_, other.Start_ + align, EPS);
    AddEdge_(other.Finish_ + align, Finish_, EPS);
    return *this;
}


TNFAutomaton operator+(const TNFAutomaton& first, const TNFAutomaton& second) {
    TNFAutomaton res = first;
    res += second;
    return res;
}


TNFAutomaton& TNFAutomaton::operator*=(const TNFAutomaton& other) {
    int align = ExtendBy_(other);
    AddEdge_(Finish_, other.Start_ + align, EPS);
    Finish_ = other.Finish_ + align;
    return *this;
}


TNFAutomaton operator*(const TNFAutomaton& first, const TNFAutomaton& second) {
    TNFAutomaton res = first;
    res *= second;
    return res;
}


TNFAutomaton TNFAutomaton::KleeneStar() const {
    TNFAutomaton res = *this;
    int newFinish = res.NewVertex_();
    res.AddEdge_(Start_, newFinish, EPS);
    res.AddEdge_(Finish_, Start_, EPS);
    res.Finish_ = newFinish;
    return res;
}


void TNFAutomaton::Iterator::EpsClosure_() {
    std::queue<int> Vqueue;
    for (int vertex : Vertexes_) { Vqueue.push(vertex); }
    while (!Vqueue.empty()) {
        int vertex = Vqueue.front();
        Vqueue.pop();
        for (const TToEdge_& edge : Nfa_.EdgesFrom_[vertex]) {
            if (edge.C == EPS && Vertexes_.count(edge.To) == 0) {
                Vqueue.push(edge.To);
                Vertexes_.insert(edge.To);
            }
        }
    }
}


void TNFAutomaton::Iterator::Next(char c) {
    std::unordered_set<int> newVertexes;
    for (int vertex : Vertexes_) {
        for (const TToEdge_& edge : Nfa_.EdgesFrom_[vertex]) {
            if (edge.C == c) { newVertexes.insert(edge.To); }
        }
    }
    Vertexes_ = newVertexes;
    EpsClosure_();
}

int TNFAutomaton::ExtendBy_(const TNFAutomaton& other) {
    int align = EdgesFrom_.size();
    EdgesFrom_.insert(EdgesFrom_.end(), other.EdgesFrom_.begin(), other.EdgesFrom_.end());
    for (size_t i = align; i < EdgesFrom_.size(); ++i) {
        for (TToEdge_& edge : EdgesFrom_[i]) { edge.To += align; }
    }
    return align;
}