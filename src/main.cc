#include <QApplication>

#include "controller/controller.h"
#include "model/model.h"
#include "view/mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  auto sm = std::make_unique<s21::SmartCalc>();
  auto cc = std::make_unique<s21::CreditCalc>();
  auto ctrl = std::make_unique<s21::Controller>(std::move(sm), std::move(cc));
  MainWindow w(std::move(ctrl));
  w.show();
  return a.exec();
}
