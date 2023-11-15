#ifndef SMART_CALC_V2_MODEL_MODEL_H_
#define SMART_CALC_V2_MODEL_MODEL_H_

#include <array>
#include <limits>
#include <ostream>
#include <string_view>
#include <utility>
#include <vector>

namespace s21 {
constexpr double EPS = 0.01;

auto assertd(double lhs, double rhs) -> bool;

class Token {
 public:
  enum class Kind {
    StartStream = -3,
    EndStream,
    Invalid,
    Whitespace,
    Number,
    Variable,
    OpenBrace,
    CloseBrace,
    PlusOp,
    MinusOp,
    MulOp,
    DivOp,
    ModOp,
    ExpOp,
    Negate,
    Function,
    Ident,
  };

 public:
  Token() = default;
  constexpr Token(Kind kind) : kind_(kind), value_("") {}
  constexpr Token(Kind kind, std::string_view value)
      : kind_(kind), value_(value) {}
  Token(const Token&) = default;
  Token(Token&&) noexcept = default;
  ~Token() = default;

 public:
  auto operator=(const Token&) -> Token& = default;
  auto operator=(Token&&) noexcept -> Token& = default;

  constexpr auto operator==(const Token& rhs) const {
    return kind_ == rhs.kind_ && value_ == rhs.value_;
  }

  constexpr auto operator!=(const Token& rhs) const {
    return kind_ != rhs.kind_ || value_ != rhs.value_;
  }

 public:
  static constexpr auto StartStream() { return Token(Kind::StartStream); }

  static constexpr auto Invalid(std::string_view tok) {
    return Token(Kind::Invalid, tok);
  }

  static constexpr auto Number(std::string_view tok) {
    return Token(Kind::Number, tok);
  }

  static constexpr auto Variable(std::string_view tok) {
    return Token(Kind::Variable, tok);
  }

  static constexpr auto Function(std::string_view tok) {
    return Token(Kind::Function, tok);
  }
  static constexpr auto Ident(std::string_view tok) {
    return Token(Kind::Ident, tok);
  }

  static constexpr auto EndStream() { return Token(Kind::EndStream); }
  static constexpr auto Invalid() { return Token(); }
  static constexpr auto Whitespace() { return Token(Kind::Whitespace); }
  static constexpr auto OpenBrace() { return Token(Kind::OpenBrace); }
  static constexpr auto CloseBrace() { return Token(Kind::CloseBrace); }
  static constexpr auto PlusOp() { return Token(Kind::PlusOp); }
  static constexpr auto MinusOp() { return Token(Kind::MinusOp); }
  static constexpr auto MulOp() { return Token(Kind::MulOp); }
  static constexpr auto DivOp() { return Token(Kind::DivOp); }
  static constexpr auto ModOp() { return Token(Kind::ModOp); }
  static constexpr auto ExpOp() { return Token(Kind::ExpOp); }
  static constexpr auto Negate() { return Token(Kind::Negate); }

 public:
  constexpr auto kind() const { return kind_; }
  constexpr auto val() const { return value_; }
  auto c_str() const -> const char*;

 public:
  auto IsOperator() const -> bool;
  constexpr auto IsIdent() const { return kind_ == Kind::Ident; }
  constexpr auto IsNumber() const { return kind_ == Kind::Number; }
  constexpr auto IsOpenBrace() const { return kind_ == Kind::OpenBrace; }

  constexpr auto IsCloseBrace() const { return kind_ == Kind::CloseBrace; }

 private:
  Kind kind_{Kind::Invalid};
  std::string_view value_;
};

std::ostream& operator<<(std::ostream& s, const Token& t);

class Lexer {
 public:
  Lexer() = default;
  Lexer(std::string_view expr)
      : expr_(expr), it_(expr.cbegin()), end_(expr.cend()) {}
  ~Lexer() = default;

 public:
  auto Next() -> Token;
  auto Collect() -> std::vector<Token>;

 private:
  auto Digit_() -> Token;
  auto Operator_() -> Token;
  auto Ident_() -> Token;

 private:
  std::string_view expr_;
  std::string_view::const_iterator it_;
  std::string_view::const_iterator end_;
  Token prev_{Token::Kind::StartStream};
};

class SmartCalc {
 public:
  using MathFn = double (*)(double);

 public:
  auto Evaluate(std::string_view, double = 0.0f) -> double;

 private:
  void Clear_();
  void Parse_(std::string_view);

 private:
  void HandleCloseBrace_();
  void HandleIdent_(const Token&);
  void HandleOperator_(const Token&);
  static constexpr auto ResolveMathFnName_(std::string_view) -> MathFn;

 private:
  std::vector<Token> ca_;
  std::vector<Token> tx_;
};

class CreditCalc {
 public:
  enum class TermType {
    Months,
    Years,
  };

  enum class CreditType {
    Annual,
    Diff,
  };

  struct Term {
    double credit_amount;
    TermType type;
    double lasting{0};
    double interest{0};
  };

  struct Result {
    std::pair<double, double> m_payment;
    double o_payment;
    double t_payment;

    auto operator==(const Result& rhs) const -> bool;
  };

 public:
  auto Evaluate(const Term& term, CreditType type) const -> Result;

 private:
  auto CalcAnnual(const Term& term) const -> Result;
  auto CalcDiff(const Term& term) const -> Result;

 private:
  Term term_;
};
}  // namespace s21

#endif
