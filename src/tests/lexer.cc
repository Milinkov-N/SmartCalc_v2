#include <gtest/gtest.h>

#include <tuple>
#include <vector>

#include "model.h"

using s21::Token;

TEST(Token, Cstr) {
  for (int i = -3; i != 15; ++i)
    std::ignore = Token(static_cast<Token::Kind>(i)).c_str();
}

TEST(Lexer, Integer) {
  s21::Lexer lexer("101");
  ASSERT_EQ(lexer.Next(), Token::Number("101"));
}

TEST(Lexer, Float) {
  s21::Lexer lexer("101.32");
  ASSERT_EQ(lexer.Next(), Token::Number("101.32"));
}

TEST(Lexer, InvalidNumber) {
  s21::Lexer lexer("101.32.23");
  ASSERT_EQ(lexer.Next(), Token::Number("101.32"));
  ASSERT_EQ(lexer.Next(), Token::Invalid("."));
}

TEST(Lexer, OpenBrace) {
  s21::Lexer lexer("(");
  ASSERT_EQ(lexer.Next(), Token::OpenBrace());
}

TEST(Lexer, CloseBrace) {
  s21::Lexer lexer("()");
  std::ignore = lexer.Next();
  ASSERT_EQ(lexer.Next(), Token::CloseBrace());
}

TEST(Lexer, CloseBraceAtStart) {
  s21::Lexer lexer(")");
  ASSERT_EQ(lexer.Next(), Token::Invalid());
}

TEST(Lexer, PlusOp) {
  s21::Lexer lexer("1+1");
  std::ignore = lexer.Next();
  ASSERT_EQ(lexer.Next(), Token::PlusOp());
}

TEST(Lexer, PlusOpAtStart) {
  s21::Lexer lexer("+");
  ASSERT_EQ(lexer.Next(), Token::Whitespace());
}

TEST(Lexer, MinusOp) {
  s21::Lexer lexer("1-1");
  std::ignore = lexer.Next();
  ASSERT_EQ(lexer.Next(), Token::MinusOp());
}

TEST(Lexer, MulOp) {
  s21::Lexer lexer("1*1");
  std::ignore = lexer.Next();
  ASSERT_EQ(lexer.Next(), Token::MulOp());
}

TEST(Lexer, MulOpAtStart) {
  s21::Lexer lexer("*");
  ASSERT_EQ(lexer.Next(), Token::Invalid());
}

TEST(Lexer, DivOp) {
  s21::Lexer lexer("1/1");
  std::ignore = lexer.Next();
  ASSERT_EQ(lexer.Next(), Token::DivOp());
}

TEST(Lexer, DivOpAtStart) {
  s21::Lexer lexer("/");
  ASSERT_EQ(lexer.Next(), Token::Invalid());
}

TEST(Lexer, ModOp) {
  s21::Lexer lexer("1%1");
  std::ignore = lexer.Next();
  ASSERT_EQ(lexer.Next(), Token::ModOp());
}

TEST(Lexer, ModOpAtStart) {
  s21::Lexer lexer("%");
  ASSERT_EQ(lexer.Next(), Token::Invalid());
}

TEST(Lexer, ExpOp) {
  s21::Lexer lexer("1^1");
  std::ignore = lexer.Next();
  ASSERT_EQ(lexer.Next(), Token::ExpOp());
}

TEST(Lexer, ExpOpAtStart) {
  s21::Lexer lexer("^");
  ASSERT_EQ(lexer.Next(), Token::Invalid());
}

TEST(Lexer, Negate) {
  s21::Lexer lexer("-1");
  ASSERT_EQ(lexer.Next(), Token::Negate());
}

TEST(Lexer, EndStream) {
  s21::Lexer lexer("1");
  std::ignore = lexer.Next();
  ASSERT_EQ(lexer.Next(), Token::EndStream());
}

TEST(Lexer, IdentSinglecharacter) {
  s21::Lexer lexer("x");
  ASSERT_EQ(lexer.Next(), Token::Ident("x"));
}

TEST(Lexer, IdentMulticharacter) {
  s21::Lexer lexer("cos()");
  ASSERT_EQ(lexer.Next(), Token::Ident("cos"));
}

TEST(Lexer, Collect) {
  s21::Lexer lexer("-+cos(1 + sin(x)) / 2");
  std::vector<Token> expect{
      Token::Negate(),     Token::Whitespace(), Token::Ident("cos"),
      Token::OpenBrace(),  Token::Number("1"),  Token::PlusOp(),
      Token::Ident("sin"), Token::OpenBrace(),  Token::Ident("x"),
      Token::CloseBrace(), Token::CloseBrace(), Token::DivOp(),
      Token::Number("2")};
  auto expect_it = expect.cbegin();

  for (auto& t : lexer.Collect()) ASSERT_EQ(t, *(expect_it++));
}
