#include <string>
#include <vector>
#include "gtest/gtest.h"
#include <iostream>
#include "lexer.h"
#include "expression.h"
#include "scope.h"
using namespace std;
using namespace mlplus;

TEST(epressionTest, evaluateTest){
    string str = "-1 + +2";
    Expression ex;
    double v = ex.evaluate(str.c_str());
    EXPECT_EQ(v, 1);

    v = ex.evaluate("log(10)");
    EXPECT_EQ(v, log(10));

    v = ex.evaluate("1 + log(10) * 2");
    EXPECT_EQ(v, 1+log(10)*2);
}
TEST(expressionTest, addTesting){
    string str = "-a + +b";
    Lexer lexer(str.c_str());
    Token* head = lexer.getTokenHead();
    Expression ex;
    std::vector<Token*> postExpression;
    ex.parse(head,  postExpression);
    char* op[] = {"a", "-","b", "+", "+"};
    EXPECT_EQ(5u, postExpression.size());
    for (unsigned i = 0; i < postExpression.size(); ++i)
    {
        EXPECT_STREQ(op[i], postExpression[i]->str);
    }

    TokenType tt[] = {OP_VAR,OP_MINUS_1, OP_VAR, OP_ADD_1, OP_ADD};
    for (unsigned i = 0; i < postExpression.size(); ++i)
    {
        EXPECT_EQ(tt[i], postExpression[i]->type);
    }
    EXPECT_TRUE(ex.verify(postExpression));
}

TEST(expressionTest, abracketTesting){
    string str = "h*(d+e)";
    Lexer lexer(str.c_str());
    Token* head = lexer.getTokenHead();
    Expression ex;
    std::vector<Token*> postExpression;
    ex.parse(head,  postExpression);
    EXPECT_EQ(5u, postExpression.size());
    char* pos[] = {"h", "d","e","+","*"};
    for (unsigned i = 0; i < postExpression.size(); ++i)
    {
        EXPECT_STREQ(pos[i], postExpression[i]->str);
    }
}

TEST(expressionTest, bracketTesting){
    string str = "a+b * c + h*(d+e)/sin(x)*cos(y)";
    Lexer lexer(str.c_str());
    Token* head = lexer.getTokenHead();
    char* op[] = {"a", "+","b", "*", "c","+","h","*","(","d","+","e",")","/","sin","(","x",")","*","cos","(","y",")"};

    Token* tail = head;
    int j = 0;
    while(tail)
    {
        EXPECT_STREQ(op[j], tail->str);
        tail = tail->next;
        ++j;
    }
    Expression ex;
    std::vector<Token*> postExpression;
    ex.parse(head,  postExpression);
    EXPECT_EQ(17u, postExpression.size());
    char* pos[] = {"a", "b","c","*","+","h","d","e","+","*","x","sin","/","y","cos","*","+"};
    for (unsigned i = 0; i < postExpression.size(); ++i)
    {
        EXPECT_STREQ(pos[i], postExpression[i]->str);
    }
}
