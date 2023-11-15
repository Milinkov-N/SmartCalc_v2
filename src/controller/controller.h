#ifndef SMART_CALC_V2_CONTROLLER_CONTROLLER_H_
#define SMART_CALC_V2_CONTROLLER_CONTROLLER_H_

#include <string_view>

#include "model/model.h"

namespace s21 {
class Controller {
 private:
  using Term = CreditCalc::Term;
  using CreditType = CreditCalc::CreditType;
  using Result = CreditCalc::Result;
  using ScPtr = std::unique_ptr<SmartCalc>;
  using CcPtr = std::unique_ptr<CreditCalc>;

 public:
  Controller() = default;
  Controller(ScPtr calc_model, CcPtr credit_model)
      : calc_(std::move(calc_model)), credit_(std::move(credit_model)) {}
  Controller(const Controller&) = delete;
  Controller(Controller&& other) = default;
  ~Controller() = default;

 public:
  auto operator=(const Controller&) -> Controller& = delete;
  auto operator=(Controller&& rhs) -> Controller& = default;

 public:
  inline auto Eval(std::string_view expr, double x) -> double {
    return calc_->Evaluate(expr, x);
  }

  inline auto CalcCredit(const Term& term, CreditType type) -> Result {
    return credit_->Evaluate(term, type);
  }

 private:
  ScPtr calc_;
  CcPtr credit_;
};
}  // namespace s21

#endif  // SMART_CALC_V2_CONTROLLER_CONTROLLER_H_
