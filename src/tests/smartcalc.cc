#include <gtest/gtest.h>

#include <cmath>
#include <vector>

#include "model.h"

using s21::SmartCalc;

TEST(SmartCalc, JustANumber) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("1"), 1);
}

TEST(SmartCalc, OnePlusOne) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("1+1"), 2);
}

TEST(SmartCalc, MinusOnePlusOne) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("-1+1"), 0);
}

TEST(SmartCalc, OnePlusMinusOne) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("1+-1"), 0);
}

TEST(SmartCalc, TenMinusEight) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("10-8"), 2);
}

TEST(SmartCalc, TenTimesEight) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("10*8"), 80);
}

TEST(SmartCalc, TenDivideEight) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("10/8"), 1.25);
}

TEST(SmartCalc, TenModuloFive) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("10%5"), 0);
}

TEST(SmartCalc, TenExpTwo) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("10^2"), 100);
}

TEST(SmartCalc, Float) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("1.2345"), 1.2345);
}

TEST(SmartCalc, InvalidFloat) {
  SmartCalc calc;
  EXPECT_THROW(calc.Evaluate("1.2345.3"), std::logic_error);
}

TEST(SmartCalc, DivisionByZero) {
  SmartCalc calc;
  EXPECT_DOUBLE_EQ(calc.Evaluate("1 / 0"), INFINITY);
}

TEST(SmartCalc, FnSin) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("sin(3.14)"), sin(3.14));
}

TEST(SmartCalc, FnCos) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("cos(3.14)"), cos(3.14));
}

TEST(SmartCalc, FnTan) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("tan(3.14)"), tan(3.14));
}

TEST(SmartCalc, FnAsin) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("asin(0.25)"), asin(0.25));
}

TEST(SmartCalc, FnAcos) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("acos(0.25)"), acos(0.25));
}

TEST(SmartCalc, FnAtan) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("atan(3.14)"), atan(3.14));
}

TEST(SmartCalc, FnSqrt) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("sqrt(3.14)"), sqrt(3.14));
}

TEST(SmartCalc, FnSqrtNegative) {
  SmartCalc calc;

  auto result = calc.Evaluate("sqrt(-3.14)");
  auto expected = sqrt(-3.14);

  ASSERT_EQ(result != result, expected != expected);
}

TEST(SmartCalc, FnLn) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("ln(3.14)"), log10(3.14));
}

TEST(SmartCalc, FnLog) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("log(3.14)"), log(3.14));
}

TEST(SmartCalc, Variable) {
  SmartCalc calc;
  ASSERT_DOUBLE_EQ(calc.Evaluate("+x", 3.14), 3.14);
}

TEST(SmartCalc, CosOnePlusTwo) {
  SmartCalc calc;
  double expected = cos(1) + 2;
  ASSERT_DOUBLE_EQ(calc.Evaluate("cos(1) + 2"), expected);
}

TEST(SmartCalc, ComplexExpression01) {
  SmartCalc calc;
  double x = 0.25, y = 10, PI = 3.14;
  double expected = (pow(cos(PI), y) + tan(x));
  ASSERT_DOUBLE_EQ(calc.Evaluate("(cos(3.14) ^ 10 + tan(x))", x), expected);
}

TEST(SmartCalc, ComplexExpression02) {
  SmartCalc calc;
  double x = 0.25, y = 10, PI = 3.14;
  double expected = -(pow(-cos(PI), y) + tan(x));
  ASSERT_DOUBLE_EQ(calc.Evaluate("-(-cos(3.14) ^ 10 + tan(x))", x), expected);
}

TEST(SmartCalc, ComplexExpressionXX) {
  SmartCalc calc;
  double x = 0.25, y = 10, PI = 3.14;
  double expected = sin(x * 12.5) - (pow(cos(PI), y) + tan(x));
  ASSERT_DOUBLE_EQ(calc.Evaluate("sin(x*12.5)-(cos(3.14)^10+tan(x))", x),
                   expected);
}
