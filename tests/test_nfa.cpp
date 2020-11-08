#include <gtest/gtest.h>
#include <vector>

#include "nfa.h"


TEST(NFATest, Constructors) {
    TNFAutomaton nfa{};
    ASSERT_EQ(nfa.GetFinish(), nfa.GetStart());

    TNFAutomaton nfa_b{'b'};
    ASSERT_NE(nfa_b.GetStart(), nfa_b.GetFinish());

    nfa_b.SetStart(nfa_b.GetFinish());
    ASSERT_EQ(nfa_b.GetFinish(), nfa_b.GetStart());
}


TEST(NFATest, AddEdge) {
    TNFAutomaton nfa{};

    auto vertex = nfa.NewVertex();
    auto finish = nfa.NewVertex();
    nfa.SetFinish(finish);
    assert(nfa.GetFinish() == finish);
    nfa.GetStart().AddEdge(vertex, 'c');
    vertex.AddEdge(finish, 'd');

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


    nfa = nfa.KleeneStar();
    ASSERT_TRUE(nfa.Accept("acaabcbaba"));

    nfa += TNFAutomaton('a') * TNFAutomaton('b').KleeneStar();
    ASSERT_TRUE(nfa.Accept("a"));

    ASSERT_TRUE(nfa.Accept("abb"));
    ASSERT_TRUE(nfa.Accept("abb"));
    ASSERT_TRUE(nfa.Accept("abbbb"));
}


TEST(NFATest, KleeneStar) {
    TNFAutomaton nfa('a');// a
    ASSERT_FALSE(nfa.Accept("aa"));
    ASSERT_TRUE(nfa.KleeneStar().Accept(""));// a*
    ASSERT_TRUE(nfa.KleeneStar().Accept("a"));
}


TEST(NFATest, DFS) {
    class TDFSVisitor {
        std::vector<char> Used_;
        int LastVertex_ = -1;
    public:
        bool Error = false;
        TDFSVisitor(const TNFAutomaton& nfa) : Used_(nfa.VertexCount(), false) {}
        void ProcessVertex(TConstNFAVertex vertex) {
            Used_[vertex] = true;
            Error |= LastVertex_ == vertex;
            LastVertex_ = vertex;
        }
        bool ProcessEdge(TConstNFAEdge edge) {
            Error |= LastVertex_ != edge.GetFrom();
            return !Used_[edge.GetTo()];
        }

        void ReturnByEdge(TConstNFAEdge edge) {
            Error |= LastVertex_ != edge.GetTo();
            LastVertex_ = edge.GetFrom();
        }

        bool AllReachable() {
            return std::all_of(Used_.begin(), Used_.end(), [](char used_i) { return used_i; });
        }
    };

    ASSERT_EQ(1, 1);
    TNFAutomaton nfa('a');
    nfa += TNFAutomaton('b') * TNFAutomaton('a');
    nfa = nfa.KleeneStar();
    nfa *= TNFAutomaton('a') * TNFAutomaton('g').KleeneStar();

    TDFSVisitor visitor(nfa);
    nfa.VisitDFS(visitor);
    ASSERT_TRUE(visitor.AllReachable());
    ASSERT_FALSE(visitor.Error);
}

TEST(NFATest, VertexCountChecks) {
    ASSERT_EQ(TNFAutomaton{}.VertexCount(), 1lu);
    ASSERT_EQ(TNFAutomaton{'a'}.VertexCount(), 2lu);

    ASSERT_EQ((TNFAutomaton{'a'} + TNFAutomaton{'b'}).VertexCount(), 4lu);
    ASSERT_EQ((TNFAutomaton{'a'} * TNFAutomaton{'b'}).VertexCount(), 4lu);
    ASSERT_EQ(TNFAutomaton{'a'}.KleeneStar().VertexCount(), 3lu);
}