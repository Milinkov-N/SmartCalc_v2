#include "plotgraph.h"

#include "ui_plotgraph.h"

PlotGraph::PlotGraph(QWidget *parent) : QDialog(parent), ui(new Ui::PlotGraph) {
  ui->setupUi(this);
}

PlotGraph::~PlotGraph() { delete ui; }

void PlotGraph::Construct(const std::unique_ptr<s21::Controller> &ctrl,
                          double xmax, double ymax, const QString &input) {
  QPen pen;
  QVector<double> x_vec, y_vec;

  double x = -xmax;
  while (x < xmax) {
    double y = ctrl->Eval(input.toStdString().c_str(), x);
    if (fabs(y) < ymax) {
      x_vec.push_back(x);
      y_vec.push_back(y);
    }
    x += 0.1;
  }

  pen.setColor(QColor(52, 237, 148));
  ui->widget->xAxis->setRange(-xmax, xmax);
  ui->widget->yAxis->setRange(-ymax, ymax);
  ui->widget->addGraph()->setPen(pen);
  ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);
  ui->widget->graph(0)->setScatterStyle(
      QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
  ui->widget->graph(0)->addData(x_vec, y_vec);
  ui->widget->setInteraction(QCP::iRangeZoom, true);
  ui->widget->setInteraction(QCP::iRangeDrag, true);
  ui->widget->setBackground(QColor(21, 30, 37));
  ui->widget->xAxis->setTickLabelColor(Qt::white);
  ui->widget->yAxis->setTickLabelColor(Qt::white);
  ui->widget->replot();
}
