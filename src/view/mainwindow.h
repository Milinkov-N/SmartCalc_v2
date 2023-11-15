#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <vector>

#include "controller/controller.h"
#include "model/model.h"
#include "plotgraph.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  MainWindow(std::unique_ptr<s21::Controller> ctrl, QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void PushToken();
  void PushFn();
  void PushPi();
  void ClearInput();
  void ClearLastInput();
  void EvaluateResult();
  void EvaluateCredit();
  void BuildGraph();

 private:
  void Setup();
  void ConnectBtn(QObject *);
  void ConnectFn(QObject *);

 private:
  Ui::MainWindow *ui;
  std::unique_ptr<s21::Controller> ctrl_;
  QString input_;
  std::vector<size_t> input_history_;
};
#endif  // MAINWINDOW_H
