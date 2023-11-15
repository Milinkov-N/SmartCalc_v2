#include <gtest/gtest.h>

#include <iostream>

#include "model.h"

using Credit = s21::CreditCalc::CreditType;
using Term = s21::CreditCalc::Term;
using TermType = s21::CreditCalc::TermType;
using Result = s21::CreditCalc::Result;

TEST(CreditCalc, AnnualMonths) {
  s21::CreditCalc cc;

  Term term;

  term.credit_amount = 300000;
  term.type = TermType::Months;
  term.lasting = 12;
  term.interest = 7.5;

  Result expected = {{26027.23, 0}, 12326.70, 312326.70};

  auto result = cc.Evaluate(term, Credit::Annual);

  ASSERT_EQ(expected, result);
}

TEST(CreditCalc, AnnualYears) {
  s21::CreditCalc cc;

  Term term;

  term.credit_amount = 300000;
  term.type = TermType::Years;
  term.lasting = 1;
  term.interest = 7.5;

  Result expected = {{26027.23, 0}, 12326.70, 312326.70};

  auto result = cc.Evaluate(term, Credit::Annual);

  ASSERT_EQ(expected, result);
}

TEST(CreditCalc, DiffMonths) {
  s21::CreditCalc cc;

  Term term;

  term.credit_amount = 300000;
  term.type = TermType::Months;
  term.lasting = 12;
  term.interest = 7.5;

  Result expected = {{26875.00, 25156.25}, 12187.50, 312187.50};

  auto result = cc.Evaluate(term, Credit::Diff);

  ASSERT_EQ(expected, result);
}

TEST(CreditCalc, DiffYears) {
  s21::CreditCalc cc;

  Term term;

  term.credit_amount = 300000;
  term.type = TermType::Years;
  term.lasting = 1;
  term.interest = 7.5;

  Result expected = {{26875.00, 25156.25}, 12187.50, 312187.50};

  auto result = cc.Evaluate(term, Credit::Diff);

  ASSERT_EQ(expected, result);
}
