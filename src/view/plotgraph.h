#ifndef PLOTGRAPH_H
#define PLOTGRAPH_H

#include <QDialog>
#include <memory>

#include "controller/controller.h"

namespace Ui {
class PlotGraph;
}

class PlotGraph : public QDialog {
  Q_OBJECT

 public:
  explicit PlotGraph(QWidget *parent = nullptr);
  ~PlotGraph();

  void Construct(const std::unique_ptr<s21::Controller> &ctrl, double xmax,
                 double ymax, const QString &input);

 private:
  Ui::PlotGraph *ui;
};

#endif  // PLOTGRAPH_H
