#pragma once

#include <QtWidgets/QMainWindow>
#include <histogram/histogram.h>

#include "ui_QtWidgetsApplication1.h"

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT
public:
    QtWidgetsApplication1(QWidget *parent = Q_NULLPTR);
    void OnBrowseClicked(bool checked);
    void OnApplyHistogramClicked(bool checked);
    void OnApplyAdaptiveHistogramClicked(bool checked);

private:
    Ui::QtWidgetsApplication1Class ui;

    std::shared_ptr<imgproc::HistogramHandler> m_histogramHandler;
    std::string m_inputImagePath = "";
    double m_clipLimit = 0.0;
};
