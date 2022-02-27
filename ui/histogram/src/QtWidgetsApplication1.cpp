#include <qapplication.h>
#include <qdesktopwidget.h>
#include <qfiledialog.h>
#include <qmessagebox.h>

#include <file/file.h>
#include <ui-image-conversions/ui-image-conversions.h>
#include <opencv2/imgcodecs.hpp>

#include "histogram/QtWidgetsApplication1.h"

QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QRect rec = QApplication::desktop()->screenGeometry();
    auto height = rec.height();
    auto width = rec.width();
    ui.centralWidget->setFixedSize(width, height);
    ui.InputImagePath->setText("");
    ui.Image1->setAlignment(Qt::AlignCenter);
    ui.Image2->setAlignment(Qt::AlignCenter);
    m_clipLimit = ui.ClipLimit->value();

    m_histogramHandler = std::make_shared<imgproc::HistogramHandler>();

    connect(ui.Browse, &QToolButton::clicked, this, &QtWidgetsApplication1::OnBrowseClicked);
    connect(ui.ApplyHistogram, &QPushButton::clicked, this, &QtWidgetsApplication1::OnApplyHistogramClicked);
    connect(ui.ApplyAdaptiveHistogram, &QPushButton::clicked, this, &QtWidgetsApplication1::OnApplyAdaptiveHistogramClicked);
    connect(ui.ClipLimit, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &QtWidgetsApplication1::OnClipLimitChanged);
}

void 
QtWidgetsApplication1::OnBrowseClicked(bool checked) {
    auto resourceDir = base::File::GetExecutablePath() / ".." / ".." / ".." / ".." / ".."
        / "image-processing" / "histogram" / "resource";
    auto fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), QString::fromStdString(resourceDir), tr("Image Files (*.png *.jpg *.bmp)"));
    if (fileName != "") {
        ui.InputImagePath->setText(fileName);
        m_inputImagePath = fileName.toStdString();
        ui.Image1->clear();
        ui.Image2->clear();
    }
}

void 
QtWidgetsApplication1::OnApplyHistogramClicked(bool checked) {
    if (m_inputImagePath == "") {
        QMessageBox::critical(this, "Path", "Input image path can not be empty", QMessageBox::Ok);
        return;
    }
    if (!base::File::FileExists(m_inputImagePath)) {
        QMessageBox::critical(this, "Path", "File does not exist in the specified path", QMessageBox::Ok);
        return;
    }
    cv::Mat inputImage = cv::imread(m_inputImagePath.c_str());
    if (inputImage.empty()) {
        QMessageBox::critical(this, "Image", "Image could not be read", QMessageBox::Ok);
        return;
    }

    auto resultImage = m_histogramHandler->ApplyHistogramEqualization(inputImage);

    inputImage = ui::UiImageConversion::AdjustImageSize(inputImage);
    auto inputPixmap = ui::UiImageConversion::CvMatToQPixmap(inputImage);
    ui.Image1->setPixmap(inputPixmap);

    resultImage = ui::UiImageConversion::AdjustImageSize(resultImage);
    auto outputPixmap = ui::UiImageConversion::CvMatToQPixmap(resultImage);
    ui.Image2->setPixmap(outputPixmap);
}

void 
QtWidgetsApplication1::OnApplyAdaptiveHistogramClicked(bool checked) {
    if (m_inputImagePath == "") {
        QMessageBox::critical(this, "Path", "Input image path can not be empty", QMessageBox::Ok);
        return;
    }
    if (!base::File::FileExists(m_inputImagePath)) {
        QMessageBox::critical(this, "Path", "File does not exist in the specified path", QMessageBox::Ok);
        return;
    }
    cv::Mat inputImage = cv::imread(m_inputImagePath.c_str());
    if (inputImage.empty()) {
        QMessageBox::critical(this, "Image", "Image could not be read", QMessageBox::Ok);
        return;
    }

    auto resultImage = m_histogramHandler->ApplyAdaptiveHistogramEqualization(inputImage, m_clipLimit);

    inputImage = ui::UiImageConversion::AdjustImageSize(inputImage);
    auto inputPixmap = ui::UiImageConversion::CvMatToQPixmap(inputImage);
    ui.Image1->setPixmap(inputPixmap);

    resultImage = ui::UiImageConversion::AdjustImageSize(resultImage);
    auto outputPixmap = ui::UiImageConversion::CvMatToQPixmap(resultImage);
    ui.Image2->setPixmap(outputPixmap);
}

void 
QtWidgetsApplication1::OnClipLimitChanged(double value) {
    m_clipLimit = value;
}