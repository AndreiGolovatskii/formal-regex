#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "nfa.h"

class NFATest: public ::testing::Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};


TEST_F(NFATest, Constructors) {
    TNFAutomaton nfa{};
    ASSERT_EQ(nfa.GetFinish(),nfa.GetStart());

    TNFAutomaton nfa_b{'b'};
    ASSERT_NE(nfa_b.GetStart(), nfa_b.GetFinish());
}

TEST_F(NFATest, AddEdge) {
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

TEST_F(NFATest, Operations) {
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

TEST_F(NFATest, KleeneStar) {
    TNFAutomaton nfa('a');  // a
    ASSERT_FALSE(nfa.Accept("aa"));
    ASSERT_TRUE(nfa.KleeneStar().Accept(""));  // a*
    ASSERT_TRUE(nfa.KleeneStar().Accept("a"));
}

TEST_F(NFATest, DFS) {
    class TDFSVisitor {
        std::vector<char> used;

    public:
        TDFSVisitor(const TNFAutomaton& nfa): used(nfa.VertexCount(), false) {}
        void ProcessVertex(TConstNFAVertex vertex) {
            used[vertex] = true;
        }
        bool ProcessEdge(TConstNFAEdge edge) {
            return !used[edge.GetTo()];
        }

        void ReturnByEdge(TConstNFAEdge edge) {
            (void)edge;
        }

        bool AllReachable() {
            return std::all_of(used.begin(), used.end(), [](char used_i){return used_i;});
        }
    };

    TNFAutomaton nfa('a');
    nfa += TNFAutomaton('b') * TNFAutomaton('a');
    nfa = nfa.KleeneStar();
    nfa *= TNFAutomaton('a') * TNFAutomaton('g').KleeneStar();

    TDFSVisitor visitor(nfa);
    nfa.VisitDFS(visitor);
    ASSERT_TRUE(visitor.AllReachable());
}