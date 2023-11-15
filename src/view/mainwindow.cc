#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  Setup();
}

MainWindow::MainWindow(std::unique_ptr<s21::Controller> ctrl, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), ctrl_(std::move(ctrl)) {
  Setup();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::PushToken() {
  auto btn = (QPushButton *)sender();
  input_.push_back(btn->text());
  input_history_.push_back(btn->text().length());
  ui->labelInput->setText(input_);
}

void MainWindow::PushFn() {
  auto btn = (QPushButton *)sender();
  input_.push_back(btn->text() + "(");
  input_history_.push_back(btn->text().length() + 1);
  ui->labelInput->setText(input_);
}

void MainWindow::PushPi() {
  input_.push_back("3.1415926");
  input_history_.push_back(9);
  ui->labelInput->setText(input_);
}

void MainWindow::ClearInput() {
  input_.clear();
  input_history_.clear();
  ui->labelInput->setText("");
  ui->labelResult->setText("0");
}

void MainWindow::ClearLastInput() {
  input_.truncate(input_.length() - input_history_.back());
  input_history_.pop_back();
  ui->labelInput->setText(input_);
}

void MainWindow::EvaluateResult() {
  if (input_.isEmpty()) return;

  try {
    double x = ui->sbX->value();
    double result = ctrl_->Eval(input_.toStdString().c_str(), x);

    if (result != result)
      ui->labelResult->setText("NaN");
    else if (result == INFINITY)
      ui->labelResult->setText("Infinity");
    else if (result == -INFINITY)
      ui->labelResult->setText("-Infinity");
    else
      ui->labelResult->setText(QString::number(result, 'g', 16));
  } catch (std::exception &e) {
    ui->labelResult->setText(QString("Error: ") + e.what());
  }
}

void MainWindow::EvaluateCredit() {
  s21::CreditCalc::Term term;

  term.type =
      static_cast<s21::CreditCalc::TermType>(ui->termType->currentIndex());
  term.credit_amount = ui->creditAmount->value();
  term.lasting = static_cast<double>(ui->termAmount->value());
  term.interest = ui->interestRate->value();

  switch (ui->creditType->currentIndex()) {
    case 0: {
      auto result =
          ctrl_->CalcCredit(term, s21::CreditCalc::CreditType::Annual);

      ui->resultMonthlyPayment->setText(
          QString::asprintf("%.2lf", result.m_payment.first));
      ui->resultOverpayment->setText(
          QString::asprintf("%.2lf", result.o_payment));
      ui->resultTotalCredit->setText(
          QString::asprintf("%.2lf", result.t_payment));
      break;
    }
    case 1: {
      auto result = ctrl_->CalcCredit(term, s21::CreditCalc::CreditType::Diff);

      ui->resultMonthlyPayment->setText(
          QString::asprintf("%.2lf", result.m_payment.first) + " .. " +
          QString::asprintf("%.2lf", result.m_payment.second));
      ui->resultOverpayment->setText(
          QString::asprintf("%.2lf", result.o_payment));
      ui->resultTotalCredit->setText(
          QString::asprintf("%.2lf", result.t_payment));
      break;
    }
  }
}

void MainWindow::BuildGraph() {
  if (input_.isEmpty()) return;

  PlotGraph pg;

  try {
    pg.Construct(ctrl_, ui->sbXMax->value(), ui->sbYMax->value(),
                 input_.toStdString().c_str());
    pg.exec();
  } catch (std::exception &e) {
    ui->labelResult->setText(QString("Error: ") + e.what());
  }
}

void MainWindow::Setup() {
  ui->setupUi(this);

  ConnectBtn(ui->btnZero);
  ConnectBtn(ui->btnOne);
  ConnectBtn(ui->btnTwo);
  ConnectBtn(ui->btnThree);
  ConnectBtn(ui->btnFour);
  ConnectBtn(ui->btnFive);
  ConnectBtn(ui->btnSix);
  ConnectBtn(ui->btnSeven);
  ConnectBtn(ui->btnEight);
  ConnectBtn(ui->btnNine);
  ConnectBtn(ui->btnDecPoint);
  ConnectBtn(ui->btnX);

  ConnectBtn(ui->btnSum);
  ConnectBtn(ui->btnSub);
  ConnectBtn(ui->btnMul);
  ConnectBtn(ui->btnDiv);
  ConnectBtn(ui->btnPow);
  ConnectBtn(ui->btnMod);
  ConnectBtn(ui->btnOpenBracket);
  ConnectBtn(ui->btnCloseBracket);

  ConnectFn(ui->btnSin);
  ConnectFn(ui->btnCos);
  ConnectFn(ui->btnTan);
  ConnectFn(ui->btnAsin);
  ConnectFn(ui->btnAcos);
  ConnectFn(ui->btnAtan);
  ConnectFn(ui->btnLn);
  ConnectFn(ui->btnLog);
  ConnectFn(ui->btnSqrt);

  connect(ui->btnPi, SIGNAL(clicked()), this, SLOT(PushPi()));
  connect(ui->btnAllClear, SIGNAL(clicked()), this, SLOT(ClearInput()));
  connect(ui->btnClear, SIGNAL(clicked()), this, SLOT(ClearLastInput()));
  connect(ui->btnResult, SIGNAL(clicked()), this, SLOT(EvaluateResult()));
  connect(ui->btnBuildPlot, SIGNAL(clicked()), this, SLOT(BuildGraph()));
  connect(ui->btnCalculate, SIGNAL(clicked()), this, SLOT(EvaluateCredit()));
}

void MainWindow::ConnectBtn(QObject *btn) {
  connect(btn, SIGNAL(clicked()), this, SLOT(PushToken()));
}

void MainWindow::ConnectFn(QObject *btn) {
  connect(btn, SIGNAL(clicked()), this, SLOT(PushFn()));
}
