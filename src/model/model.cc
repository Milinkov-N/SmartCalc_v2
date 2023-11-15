#include "model.h"

#include <cctype>
#include <cmath>
#include <cstring>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <tuple>

#define APPLY_OP(STACK_NAME, EXPR) \
  STACK_NAME.push_back(            \
      ApplyOperator(STACK_NAME, [](auto lhs, auto rhs) { return EXPR; }))

static auto isop(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' ||
         c == '(' || c == ')';
}

static auto ApplyOperator(std::vector<double>& stack,
                          std::function<double(double, double)> cb) {
  if (stack.size() < 2)
    throw std::invalid_argument("cannot apply operator (Stack Underflow)");

  double rhs = stack.back();
  stack.pop_back();
  double lhs = stack.back();
  stack.pop_back();
  return cb(lhs, rhs);
}

auto s21::assertd(double lhs, double rhs) -> bool {
  return fabs(lhs - rhs) < EPS;
}

auto s21::Token::c_str() const -> const char* {
  switch (kind_) {
    case Kind::StartStream:
      return "StartStream";
    case Kind::EndStream:
      return "EndStream";
    case Kind::Invalid:
      return "Invalid";
    case Kind::Whitespace:
      return "Whitespace";
    case Kind::Number:
      return "Number";
    case Kind::Variable:
      return "Variable";
    case Kind::OpenBrace:
      return "OpenBrace";
    case Kind::CloseBrace:
      return "CloseBrace";
    case Kind::PlusOp:
      return "PlusOp";
    case Kind::MinusOp:
      return "MinusOp";
    case Kind::MulOp:
      return "MulOp";
    case Kind::DivOp:
      return "DivOp";
    case Kind::ModOp:
      return "ModOp";
    case Kind::ExpOp:
      return "ExpOp";
    case Kind::Negate:
      return "Negate";
    case Kind::Function:
      return "Function";
    case Kind::Ident:
      return "Ident";
    default:
      return "???";
  }
}

auto s21::Token::IsOperator() const -> bool {
  return kind_ == Kind::Negate || kind_ == Kind::PlusOp ||
         kind_ == Kind::MinusOp || kind_ == Kind::MulOp ||
         kind_ == Kind::DivOp || kind_ == Kind::ModOp || kind_ == Kind::ExpOp;
}

std::ostream& s21::operator<<(std::ostream& s, const Token& t) {
  s << "Token::" << t.c_str();

  switch (t.kind()) {
    case Token::Kind::Invalid:
    case Token::Kind::Number:
    case Token::Kind::Ident:
    case Token::Kind::Variable:
    case Token::Kind::Function:
      s << "(" << t.val() << ")";
      break;

    default:
      break;
  }

  return s;
}

auto s21::Lexer::Next() -> Token {
  Token t;

  while (it_ != end_ && std::isspace(*it_)) it_++;
  if (it_ == end_) return Token::EndStream();

  if (std::isdigit(*it_))
    t = Digit_();
  else if (isop(*it_))
    t = Operator_();
  else if (std::isalpha(*it_))
    t = Ident_();
  else
    t = Token::Invalid({it_++, 1});

  prev_ = t;

  return t;
}

auto s21::Lexer::Collect() -> std::vector<Token> {
  std::vector<Token> tokens;
  for (auto tok = Next(); tok != Token::EndStream(); tok = Next())
    tokens.emplace_back(tok);
  return tokens;
}

auto s21::Lexer::Digit_() -> Token {
  std::size_t n = 0;
  auto start = it_;
  bool have_dec_point = false;
  for (; it_ != end_ && strchr("1234567890.", *it_); ++it_, ++n)
    if (*it_ == '.' && !have_dec_point)
      have_dec_point = true;
    else if (*it_ == '.' && have_dec_point)
      break;
  return Token::Number({start, n});
}

auto s21::Lexer::Operator_() -> Token {
  switch (*(it_++)) {
    case '+': {
      if (prev_ == Token::StartStream() || prev_.IsOperator())
        return Token::Whitespace();
      return Token::PlusOp();
    }
    case '-': {
      if (prev_ == Token::StartStream() ||
          ((prev_.IsOperator() || prev_.IsOpenBrace()) &&
           !prev_.IsCloseBrace()))
        return Token::Negate();
      return Token::MinusOp();
    }
    case '*': {
      if (prev_ == Token::StartStream()) return Token();
      return Token::MulOp();
    }
    case '/': {
      if (prev_ == Token::StartStream()) return Token();
      return Token::DivOp();
    }
    case '%': {
      if (prev_ == Token::StartStream()) return Token();
      return Token::ModOp();
    }
    case '^': {
      if (prev_ == Token::StartStream()) return Token();
      return Token::ExpOp();
    }
    case '(':
      return Token::OpenBrace();
    case ')': {
      if (prev_ == Token::StartStream()) return Token();
      return Token::CloseBrace();
    }
    default:
      return Token::Invalid({it_ - 1});
  }
}

auto s21::Lexer::Ident_() -> Token {
  std::size_t n = 0;
  auto start = it_;
  for (; it_ != end_ && std::isalnum(*it_); ++it_, ++n)
    ;
  return Token::Ident({start, n});
}

double s21::SmartCalc::Evaluate(std::string_view expr, double x) {
  Clear_();
  Parse_(expr);

  std::vector<double> stack{};

  for (auto& tok : ca_) {
    switch (tok.kind()) {
      case Token::Kind::Number:
        stack.push_back(std::atof(tok.val().data()));
        break;

      case Token::Kind::Variable:
        stack.push_back(x);
        break;

      case Token::Kind::PlusOp:
        APPLY_OP(stack, lhs + rhs);
        break;

      case Token::Kind::MinusOp:
        APPLY_OP(stack, lhs - rhs);
        break;

      case Token::Kind::MulOp:
        APPLY_OP(stack, lhs * rhs);
        break;

      case Token::Kind::DivOp:
        APPLY_OP(stack, lhs / rhs);
        break;

      case Token::Kind::ModOp:
        APPLY_OP(stack, std::fmod(lhs, rhs));
        break;

      case Token::Kind::ExpOp:
        APPLY_OP(stack, std::pow(lhs, rhs));
        break;

      case Token::Kind::Negate: {
        if (stack.size() < 1) {
          constexpr auto msg = "cannot apply negation (Stack Underflow)";
          throw std::invalid_argument(msg);
        }

        auto& val = stack.back();
        val = -val;
      } break;

      case Token::Kind::Function: {
        auto fn_ptr = ResolveMathFnName_(tok.val());

        if (fn_ptr != nullptr) {
          if (stack.size() < 1) {
            constexpr auto msg =
                "cannot evaluate function call (Stack Underflow)";
            throw std::invalid_argument(msg);
          }

          double& val = stack.back();
          val = fn_ptr(val);
        } else {
          std::stringstream ss;
          ss << "invalid function name '" << tok.val() << "'";
          throw std::logic_error(ss.str());
        }
      } break;

      default:
        std::stringstream ss;
        ss << "invalid token '" << tok << "'";
        throw std::logic_error(ss.str());
        break;
    }
  }

  return stack.back();
}

void s21::SmartCalc::Clear_() {
  ca_.clear();
  tx_.clear();
}

void s21::SmartCalc::Parse_(std::string_view expr) {
  Lexer lexer(expr);

  for (auto& t : lexer.Collect()) {
    if (t.IsNumber())
      ca_.push_back(t);
    else if (t.IsIdent())
      HandleIdent_(t);
    else if (t.IsOpenBrace())
      tx_.push_back(t);
    else if (t.IsCloseBrace())
      HandleCloseBrace_();
    else if (t.IsOperator())
      HandleOperator_(t);
    else if (t == Token::Whitespace())
      continue;
    else {
      std::stringstream ss;
      ss << "invalid token '" << t << "'";
      throw std::logic_error(ss.str());
    }
  }

  while (tx_.size()) {
    ca_.push_back(tx_.back());
    tx_.pop_back();
  }
}

void s21::SmartCalc::HandleCloseBrace_() {
  while (!tx_.empty()) {
    if (tx_.back() == Token::OpenBrace()) {
      tx_.pop_back();
      break;
    }
    ca_.push_back(tx_.back());
    tx_.pop_back();
  }
}

void s21::SmartCalc::HandleIdent_(const Token& tok) {
  if (ResolveMathFnName_(tok.val()) != nullptr)
    tx_.emplace_back(Token::Function(tok.val()));
  else if (tok.val() == "x")
    ca_.emplace_back(Token::Variable(tok.val()));
  else
    ca_.emplace_back(Token::Invalid(tok.val()));
}

void s21::SmartCalc::HandleOperator_(const Token& tok) {
  if (tx_.size() != 0) {
    auto& tx_back = tx_.back();
    if (tx_back.kind() >= tok.kind()) {
      ca_.push_back(tx_back);
      tx_.pop_back();

      if (tx_back.kind() == Token::Kind::Function && tx_.size() > 0 &&
          tx_.back() == Token::Negate()) {
        ca_.push_back(tx_.back());
        tx_.pop_back();
      }
    }
  }
  tx_.push_back(tok);
}

constexpr auto s21::SmartCalc::ResolveMathFnName_(std::string_view name)
    -> s21::SmartCalc::MathFn {
  if (name.size() == 0) return nullptr;
  MathFn ptr = nullptr;

  if (name == "cos")
    ptr = std::cos;
  else if (name == "sin")
    ptr = std::sin;
  else if (name == "tan")
    ptr = std::tan;
  else if (name == "acos")
    ptr = std::acos;
  else if (name == "asin")
    ptr = std::asin;
  else if (name == "atan")
    ptr = std::atan;
  else if (name == "sqrt")
    ptr = std::sqrt;
  else if (name == "ln")
    ptr = std::log10;
  else if (name == "log")
    ptr = std::log;

  return ptr;
}

auto s21::CreditCalc::Evaluate(const Term& term, CreditType type) const
    -> Result {
  switch (type) {
    case CreditType::Annual:
      return CalcAnnual(term);

    case CreditType::Diff:
      return CalcDiff(term);
  }

  return Result();
}

auto s21::CreditCalc::CalcAnnual(const Term& term) const -> Result {
  double interest_rate = term.interest / (100 * 12);
  double term_in_months{0};

  switch (term.type) {
    case TermType::Months:
      term_in_months = term.lasting;
      break;
    case TermType::Years:
      term_in_months = term.lasting * 12;
      break;
  }

  double m_payment =
      term.credit_amount *
      (interest_rate / (1 - pow(1 + interest_rate, -term_in_months)));
  double o_payment = (m_payment * term_in_months) - term.credit_amount;
  double t_payment = term.credit_amount + o_payment;

  return {{m_payment, 0}, o_payment, t_payment};
}

auto s21::CreditCalc::CalcDiff(const Term& term) const -> Result {
  double main_part{0};
  double overpayment{0};
  double term_in_months{0};

  switch (term.type) {
    case TermType::Months:
      term_in_months = term.lasting;
      break;
    case TermType::Years:
      term_in_months = term.lasting * 12;
      break;
  }

  main_part = term.credit_amount / term_in_months;

  std::vector<double> interest_payments;

  for (int i = 0; i < term_in_months; ++i) {
    interest_payments.push_back((term.credit_amount - main_part * i) *
                                ((term.interest / 100) / 12));
    overpayment += interest_payments.back();
  }

  return {{interest_payments.front() + main_part,
           interest_payments.back() + main_part},
          overpayment,
          term.credit_amount + overpayment};
}

auto s21::CreditCalc::Result::operator==(const Result& rhs) const -> bool {
  return assertd(m_payment.first, rhs.m_payment.first) &&
         assertd(m_payment.second, rhs.m_payment.second) &&
         assertd(o_payment, rhs.o_payment) && assertd(t_payment, rhs.t_payment);
}
