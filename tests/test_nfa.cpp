#include <gtest/gtest.h>
#include <unordered_map>

#include "nfa.h"


TEST(NFATest, Constructors) {
    TNFAutomaton nfa{TNFAutomaton::EPS};
    ASSERT_NE(nfa.GetFinish(), nfa.GetStart());
    ASSERT_TRUE(nfa.Accept(""));

    TNFAutomaton nfa_b{'b'};
    ASSERT_NE(nfa_b.GetStart(), nfa_b.GetFinish());

    nfa_b.SetStart(nfa_b.GetFinish());
    ASSERT_EQ(nfa_b.GetFinish(), nfa_b.GetStart());
}


TEST(NFATest, AddEdge) {
    TNFAutomaton nfa{TNFAutomaton::EPS};

    auto vertex = nfa.NewVertex();
    auto finish = nfa.NewVertex();
    nfa.SetFinish(finish);
    assert(nfa.GetFinish() == finish);
    nfa.GetStart()->AddEdge(vertex, 'c');
    vertex->AddEdge(finish, 'd');

    ASSERT_TRUE(nfa.Accept("cd"));
    ASSERT_FALSE(nfa.Accept("cdcd"));
    ASSERT_FALSE(nfa.Accept("c"));
    ASSERT_FALSE(nfa.Accept("d"));
}


TEST(NFATest, Operations) {
    TNFAutomaton nfa{'a'};

    nfa += TNFAutomaton('b');                    // a + b
    nfa *= TNFAutomaton('c') + TNFAutomaton('a');// (a + b) * (c + a)

    ASSERT_NE(nfa.GetFinish(), nfa.GetStart());
    ASSERT_TRUE(nfa.Accept("ac"));
    ASSERT_TRUE(nfa.Accept("aa"));
    ASSERT_TRUE(nfa.Accept("bc"));
    ASSERT_TRUE(nfa.Accept("ba"));

    ASSERT_FALSE(nfa.Accept("bb"));
    ASSERT_FALSE(nfa.Accept("a"));
    ASSERT_FALSE(nfa.Accept("b"));
    ASSERT_FALSE(nfa.Accept("c"));


    nfa.KleeneStarInplace();
    ASSERT_TRUE(nfa.Accept("acaabcbaba"));

    nfa += TNFAutomaton('a') * std::move(TNFAutomaton('b').KleeneStarInplace());
    ASSERT_TRUE(nfa.Accept("a"));

    ASSERT_TRUE(nfa.Accept("abb"));
    ASSERT_TRUE(nfa.Accept("abb"));
    ASSERT_TRUE(nfa.Accept("abbbb"));
}


TEST(NFATest, KleeneStar) {
    TNFAutomaton nfa('a');// a
    ASSERT_FALSE(nfa.Accept("aa"));
    nfa.KleeneStarInplace();
    ASSERT_TRUE(nfa.Accept(""));// a*
    ASSERT_TRUE(nfa.Accept("a"));
}


TEST(NFATest, DFS) {
    class TDFSVisitor {
        std::unordered_map<const TNFAVertex*, char> Used_;
        const TNFAVertex* LastVertex_ = nullptr;

    public:
        bool Error = false;
        void ProcessVertex(const TNFAVertex* vertex) {
            Used_[vertex] = true;
            Error |= LastVertex_ == vertex;
            LastVertex_ = vertex;
        }
        bool ProcessEdge(TNFAEdge edge) {
            Error |= LastVertex_ != edge.GetFrom();
            return !Used_[edge.GetTo()];
        }

        void ReturnByEdge(TNFAEdge edge) {
            Error |= LastVertex_ != edge.GetTo();
            LastVertex_ = edge.GetFrom();
        }

        bool AllReachable() {
            return std::all_of(Used_.begin(), Used_.end(), [](const auto& iterator) { return iterator.second; });
        }
    };

    ASSERT_EQ(1, 1);
    TNFAutomaton nfa('a');
    nfa += TNFAutomaton('b') * TNFAutomaton('a');
    nfa.KleeneStarInplace();
    nfa *= TNFAutomaton('a') * std::move(TNFAutomaton('g').KleeneStarInplace());

    TDFSVisitor visitor;
    nfa.VisitDFS(visitor);
    ASSERT_TRUE(visitor.AllReachable());
    ASSERT_FALSE(visitor.Error);
}
