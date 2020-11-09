#ifndef PRACTICUM_NFA_H
#define PRACTICUM_NFA_H

#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


class TNFAutomaton;

class TNFAVertex {
    friend TNFAutomaton;
    TNFAVertex() = default;

protected:
    struct Edge_ {
        TNFAVertex* To;
        char C;
        Edge_(TNFAVertex* to, char c) : To(to), C(c){};
    };
    std::vector<Edge_> AdjEdges_{};

public:
    void AddEdge(TNFAVertex* other, char c);
};


class TNFAEdge {
public:
    [[nodiscard]] inline const TNFAVertex* GetFrom() const { return From_; }
    [[nodiscard]] inline const TNFAVertex* GetTo() const { return To_; }

    [[nodiscard]] inline char GetC() const { return C_; }

protected:
    friend TNFAutomaton;
    friend TNFAVertex;

    const TNFAVertex* From_;
    const TNFAVertex* To_;
    char C_;
    TNFAEdge(const TNFAVertex* from, const TNFAVertex* to, char c) : From_(from), To_(to), C_(c) {}
};

class TNFAutomaton {
public:
    static const char EPS = 0;
    TNFAutomaton(const TNFAutomaton& other) = delete;
    TNFAutomaton& operator=(const TNFAutomaton& other) = delete;

    TNFAutomaton(TNFAutomaton&& other) = default;

    explicit TNFAutomaton(char c);

    [[nodiscard]] inline TNFAVertex* GetStart() { return Start_; }

    [[nodiscard]] inline const TNFAVertex* GetStart() const { return Start_; }

    [[nodiscard]] inline TNFAVertex* GetFinish() { return Finish_; }

    [[nodiscard]] inline const TNFAVertex* GetFinish() const { return Finish_; }

    [[nodiscard]] inline TNFAVertex* NewVertex() {
        auto* newVertex = new TNFAVertex();
        std::unique_ptr<TNFAVertexHolder> newTop(new TNFAVertexHolder(newVertex, std::move(VertexesListTop_)));
        VertexesListTop_ = std::move(newTop);
        if (VertexesListBottom_ == nullptr) { VertexesListBottom_ = VertexesListTop_.get(); }
        return newVertex;
    }

    inline void SetStart(TNFAVertex* vertex) { Start_ = vertex; }

    inline void SetFinish(TNFAVertex* vertex) { Finish_ = vertex; }

    TNFAutomaton& operator+=(TNFAutomaton&& other);

    TNFAutomaton& operator*=(TNFAutomaton&& other);

    TNFAutomaton& KleeneStarInplace();

    class Iterator {
        const TNFAutomaton& Nfa_;
        std::unordered_set<const TNFAVertex*> Vertexes_;

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

    inline void Clear() {
        VertexesListTop_ = nullptr;
        VertexesListBottom_ = nullptr;
        Start_ = Finish_ = nullptr;
    }

private:
    struct TNFAVertexHolder {
        std::unique_ptr<TNFAVertex> Value;
        std::unique_ptr<TNFAVertexHolder> Next;
        TNFAVertexHolder(TNFAVertex* newVertex, std::unique_ptr<TNFAVertexHolder>&& top)
            : Value(newVertex), Next(std::move(top)) {}
    };
    TNFAutomaton();

    TNFAVertex* Start_ = nullptr;
    TNFAVertex* Finish_ = nullptr;

    std::unique_ptr<TNFAVertexHolder> VertexesListTop_ = nullptr;
    TNFAVertexHolder* VertexesListBottom_ = nullptr;

    friend TNFAVertex;

    template<typename Visitor>
    void VisitDFS_(Visitor& visitor, const TNFAVertex* vertex) const;
};


template<typename Visitor>
void TNFAutomaton::VisitDFS_(Visitor& visitor, const TNFAVertex* vertex) const {
    visitor.ProcessVertex(vertex);
    for (const auto& edge : vertex->AdjEdges_) {
        if (visitor.ProcessEdge(TNFAEdge(vertex, edge.To, edge.C))) {
            VisitDFS_(visitor, edge.To);
            visitor.ReturnByEdge(TNFAEdge(vertex, edge.To, edge.C));
        }
    }
}


TNFAutomaton operator+(TNFAutomaton&& first, TNFAutomaton&& second);


TNFAutomaton operator*(TNFAutomaton&& first, TNFAutomaton&& second);


#endif//PRACTICUM_NFA_H
