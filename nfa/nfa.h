#ifndef PRACTICUM_NFA_H
#define PRACTICUM_NFA_H

#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


class TNFAutomaton;

class TConstNFAEdge;

class TConstNFAVertex {
public:
    inline operator int() const { return Id_; }

    TConstNFAVertex(const TNFAutomaton& nfa, int id);

protected:
    TNFAutomaton& Nfa_;
    const int Id_;
};


class TNFAVertex : public TConstNFAVertex {
public:
    void AddEdge(const TNFAVertex& other, char c);
    TNFAVertex(TNFAutomaton& nfa, int id) : TConstNFAVertex(nfa, id) {}
};


class TConstNFAEdge {
public:
    [[nodiscard]] inline TConstNFAVertex GetFrom() const { return TConstNFAVertex(Nfa_, From_); }

    [[nodiscard]] inline TConstNFAVertex GetTo() const { return TConstNFAVertex(Nfa_, To_); }

    [[nodiscard]] inline char GetC() const { return C_; }

protected:
    friend TNFAutomaton;
    friend TNFAVertex;

    TNFAutomaton& Nfa_;
    int From_;
    int To_;
    char C_;
    TConstNFAEdge(const TNFAutomaton& nfa, int from, int to, char c);
};


class TNFAutomaton {
public:
    static const char EPS = 0;

    TNFAutomaton();

    explicit TNFAutomaton(char c);

    [[nodiscard]] inline size_t VertexCount() const { return EdgesFrom_.size(); }

    [[nodiscard]] inline TNFAVertex GetStart() { return TNFAVertex(*this, Start_); }

    [[nodiscard]] inline TConstNFAVertex GetStart() const { return TConstNFAVertex(*this, Start_); }

    [[nodiscard]] inline TNFAVertex GetFinish() { return TNFAVertex(*this, Finish_); }

    [[nodiscard]] inline TConstNFAVertex GetFinish() const { return TConstNFAVertex(*this, Finish_); }

    [[nodiscard]] inline TNFAVertex NewVertex() { return {*this, NewVertex_()}; }

    inline void SetStart(TNFAVertex vertex) { Start_ = vertex; }

    inline void SetFinish(TNFAVertex vertex) { Finish_ = vertex; }

    TNFAutomaton& operator+=(const TNFAutomaton& other);

    TNFAutomaton& operator*=(const TNFAutomaton& other);

    [[nodiscard]] TNFAutomaton KleeneStar() const;

    class Iterator {
        const TNFAutomaton& Nfa_;
        std::unordered_set<int> Vertexes_;

        void EpsClosure_();

    public:
        explicit Iterator(const TNFAutomaton& nfa) : Nfa_(nfa), Vertexes_({nfa.Start_}) { EpsClosure_(); }

        void Next(char c);

        [[nodiscard]] inline bool IsTerminal() const { return Vertexes_.count(Nfa_.Finish_); }
    };

    [[nodiscard]] bool Accept(const std::string& str) const {
        Iterator iter = Iterator(*this);
        for (const auto& letter : str) { iter.Next(letter); }
        return iter.IsTerminal();
    }

    template<typename Visitor>
    void VisitDFS(Visitor& visitor) const {
        VisitDFS_(visitor, Start_);
    }

private:
    struct TToEdge_ {
        int To;
        char C;
        TToEdge_(int to, char c) : To(to), C(c) {}
    };

    int Start_;
    int Finish_;
    std::vector<std::vector<TToEdge_>> EdgesFrom_;

    friend TNFAVertex;

    inline void AddEdge_(int from, int to, char edge) { EdgesFrom_[from].emplace_back(to, edge); }
    int ExtendBy_(const TNFAutomaton& other);

    template<typename Visitor>
    void VisitDFS_(Visitor& visitor, int vertex) const;

    [[nodiscard]] inline int NewVertex_() {
        int id = EdgesFrom_.size();
        EdgesFrom_.emplace_back();
        return id;
    }
};


TNFAutomaton operator*(const TNFAutomaton& first, const TNFAutomaton& second);


TNFAutomaton operator+(const TNFAutomaton& first, const TNFAutomaton& second);


template<typename Visitor>
void TNFAutomaton::VisitDFS_(Visitor& visitor, int vertex) const {
    visitor.ProcessVertex(TConstNFAVertex(*this, vertex));
    for (const TToEdge_& edge : EdgesFrom_[vertex]) {
        if (visitor.ProcessEdge(TConstNFAEdge(*this, vertex, edge.To, edge.C))) {
            VisitDFS_(visitor, edge.To);
            visitor.ReturnByEdge(TConstNFAEdge(*this, vertex, edge.To, edge.C));
        }
    }
}

#endif//PRACTICUM_NFA_H
