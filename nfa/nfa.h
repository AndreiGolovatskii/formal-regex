#ifndef PRACTICUM_NFA_H
#define PRACTICUM_NFA_H

#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


class TNFAutomaton;

class TNFAEdge;
class TConstNFAEdge;

class TConstNFAVertex {
    friend TNFAutomaton;
    friend TNFAEdge;
    friend TConstNFAEdge;

protected:
    TNFAutomaton &Nfa_;
    const int Id_;
    TConstNFAVertex(const TNFAutomaton &nfa, int id) : Nfa_(const_cast<TNFAutomaton &>(nfa)), Id_(id) {}

public:
    operator int() const { return Id_; }
};

class TNFAVertex : public TConstNFAVertex {
    friend TNFAutomaton;
    friend TNFAEdge;

protected:
    TNFAVertex(TNFAutomaton &nfa, int id) : TConstNFAVertex(nfa, id) {}

public:
    void AddEdge(const TNFAVertex &other, char c);
};


class TConstNFAEdge {
    friend TNFAutomaton;
    friend TNFAVertex;

protected:
    TNFAutomaton &Nfa_;
    int From_;
    int To_;
    char C_;
    TConstNFAEdge(const TNFAutomaton &nfa, int from, int to, char c)
        : Nfa_(const_cast<TNFAutomaton &>(nfa)), From_(from), To_(to), C_(c) {}

public:
    TConstNFAVertex GetFrom() const { return TConstNFAVertex(Nfa_, From_); }
    TConstNFAVertex GetTo() const { return TConstNFAVertex(Nfa_, To_); }

    char GetC() const { return C_; }
};

class TNFAEdge: public TConstNFAEdge {
    friend TNFAutomaton;
    friend TNFAVertex;

protected:
    TNFAEdge(TNFAutomaton &nfa, int from, int to, char c) : TConstNFAEdge(nfa, from, to, c) {}

public:
    TNFAVertex GetFrom() { return TNFAVertex(Nfa_, From_); }
    TNFAVertex GetTo() { return TNFAVertex(Nfa_, To_); }
};

class TNFAutomaton {
    int Start_;
    int Finish_;
    friend TNFAVertex;
    friend TNFAEdge;

    struct TToEdge_ {
        int To;
        char C;
        TToEdge_(int to, char c) : To(to), C(c) {}
    };

    std::vector<std::vector<TToEdge_>> EdgesFrom;

    void AddEdge_(int from, int to, char edge) {
        assert(from < static_cast<int>(EdgesFrom.size()));
        assert(to < static_cast<int>(EdgesFrom.size()));

        EdgesFrom[from].emplace_back(to, edge);
    }

    int ExtendBy(const TNFAutomaton &other) {
        int align = EdgesFrom.size();
        EdgesFrom.insert(EdgesFrom.end(), other.EdgesFrom.begin(), other.EdgesFrom.end());
        for (size_t i = align; i < EdgesFrom.size(); ++i) {
            for (TToEdge_ &edge : EdgesFrom[i]) { edge.To += align; }
        }
        return align;
    }

    template<typename Visitor>
    void VisitDFS_(Visitor &visitor, int vertex) const {
        visitor.ProcessVertex(TConstNFAVertex(*this, vertex));
        for (const TToEdge_ &edge : EdgesFrom[vertex]) {
            if (visitor.ProcessEdge(TConstNFAEdge(*this, vertex, edge.To, edge.C))) {
                VisitDFS_(visitor, edge.To);
                visitor.ReturnByEdge(TConstNFAEdge(*this, vertex, edge.To, edge.C));
            }
        }
    }

public:
    static const char EPS = 0;

    TNFAutomaton() : Start_(0), Finish_(0), EdgesFrom(1){};
    explicit TNFAutomaton(char c) : Start_(0), Finish_(1), EdgesFrom(2) { EdgesFrom[Start_].emplace_back(Finish_, c); }

    size_t VertexCount() const { return EdgesFrom.size(); }
    TNFAVertex GetStart() { return TNFAVertex(*this, Start_); }

    TConstNFAVertex GetStart() const { return TConstNFAVertex(*this, Start_); }

    TNFAVertex GetFinish() { return TNFAVertex(*this, Finish_); }

    TConstNFAVertex GetFinish() const { return TConstNFAVertex(*this, Finish_); }

    TNFAutomaton &operator+=(const TNFAutomaton &other) {
        int align = ExtendBy(other);
        AddEdge_(Start_, other.Start_ + align, EPS);
        AddEdge_(other.Finish_ + align, Finish_, EPS);
        return *this;
    }

    TNFAutomaton &operator*(const TNFAutomaton &other) {
        int align = ExtendBy(other);
        AddEdge_(Finish_, other.Start_ + align, EPS);
        Finish_ = other.Finish_ + align;
        return *this;
    }

    TNFAutomaton &KleeneStar() {
        AddEdge_(Finish_, Start_, EPS);
        return *this;
    }

    class Iterator {
        const TNFAutomaton &Nfa_;
        std::unordered_set<int> Vertexes_;

        void EpsClosure_() {
            std::queue<int> Vqueue;
            for (int vertex : Vertexes_) { Vqueue.push(vertex); }
            while (!Vqueue.empty()) {
                int vertex = Vqueue.front();
                Vqueue.pop();
                for (const TToEdge_ &edge : Nfa_.EdgesFrom[vertex]) {
                    if (edge.C == EPS && Vertexes_.count(edge.To) == 0) {
                        Vqueue.push(edge.To);
                        Vertexes_.insert(edge.To);
                    }
                }
            }
        }

    public:
        Iterator(const TNFAutomaton &nfa) : Nfa_(nfa), Vertexes_({nfa.Start_}) { EpsClosure_(); }

        void Next(char c) {
            std::unordered_set<int> newVertexes;
            for (int vertex : Vertexes_) {
                for (const TToEdge_ &edge : Nfa_.EdgesFrom[vertex]) {
                    if (edge.C == c) { newVertexes.insert(edge.To); }
                }
            }
            Vertexes_ = newVertexes;
            EpsClosure_();
        }

        bool IsTerminal() const { return Vertexes_.count(Nfa_.Finish_); }
    };

    bool Accept(const std::string &s) const {
        Iterator iter = Iterator(*this);
        for (const auto &c : s) { iter.Next(c); }
        return iter.IsTerminal();
    }

    template<typename Visitor>
    void VisitDFS(Visitor &visitor) const {
        VisitDFS_(visitor, Start_);
    }
};

void TNFAVertex::AddEdge(const TNFAVertex &other, char c) { Nfa_.AddEdge_(Id_, other.Id_, c); }

#endif//PRACTICUM_NFA_H
