#include "WinImageFilterGrid.h"
#include "WinSymbolScanner.h"
#include "qtcvHelper.h"
#include <QListWidget>
#include <QFileSystemModel>

WinImageFilterGrid::WinImageFilterGrid(QWidget *parent)
  : QMainWindowChild(parent)
{
  _ui.setupUi(this);

  // setup file listener
  _fileModel = new QFileSystemModel(this);
  _fileModel->setFilter(QDir::Files);
  _fileModel->setNameFilterDisables(false);
  _fileModel->setReadOnly(false);

  _ui.listViewFiles->setModel(_fileModel);

  connect(_ui.listViewFiles->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
          this, SLOT(onc_listViewSelectionModel_currentChanged(const QModelIndex&, const QModelIndex&)));
}

WinImageFilterGrid::~WinImageFilterGrid()
{

}

void WinImageFilterGrid::setCurrentFolder(const QString& directory)
{
  _fileModel->setNameFilters(parentMainWindow()->property("defaultFileFilter").toStringList());
  _ui.listViewFiles->clearSelection();
  _ui.listViewFiles->setRootIndex(_fileModel->setRootPath(directory));
}

void WinImageFilterGrid::onc_listViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
  auto fileName = _fileModel->filePath(current);

  QPixmap orgPix = parentMainWindow()->imageCache().value(fileName);
  cv::Mat org = qPixmapToCvMat(orgPix, false);

  //cv::Mat greyMat;
  //cv::cvtColor(org, greyMat, CV_BGR2GRAY);

  QPixmap editedPix = cvMatToQPixmap(greyMat);
  _ui.labelPreview->setPixmap(editedPix);
}

/*
void WinImageFilterGrid::refreshPreviewImage(void)
{
  if (_ui.buttonGroup->checkedButton() == _ui.radioButtonRaster)
  {
    cv::Scalar low = qColorToCvScalar(_ui.colorLineEditRasterLower->color());
    cv::Scalar up = qColorToCvScalar(_ui.colorLineEditRasterUpper->color());

    cv::cvtColor(imMatOrignal, imMatPreview, cv::COLOR_BGR2HSV);
    cv::inRange(imMatPreview, low, up, imMatPreview);
  } else if (_ui.buttonGroup->checkedButton() == _ui.radioButtonSign)
  {
    cv::Scalar low = qColorToCvScalar(_ui.colorLineEditSignsLower->color());
    cv::Scalar up = qColorToCvScalar(_ui.colorLineEditSignsUpper->color());

    cv::cvtColor(imMatOrignal, imMatPreview, cv::COLOR_BGR2HSV);
    cv::inRange(imMatPreview, low, up, imMatPreview);
  } else
  {
    imMatOrignal.copyTo(imMatPreview);
    //cv::cvtColor(imMatOrignal, imMatPreview, cv::COLOR_BGR2RGB);
  }

  _ui.labelImage->setPixmap(cvMatToQPixmap(imMatPreview));
}

void WinImageFilterGrid::on_widgetLowerFilter_colorChanged(QColor color)
{
  if (_ui.buttonGroup->checkedButton() == _ui.radioButtonRaster)
    _ui.colorLineEditRasterLower->setColor(color);
  else if (_ui.buttonGroup->checkedButton() == _ui.radioButtonSign)
    _ui.colorLineEditSignsLower->setColor(color);
  refreshPreviewImage();
}

void WinImageFilterGrid::on_widgetUpperFilter_colorChanged(QColor color)
{
  if (_ui.buttonGroup->checkedButton() == _ui.radioButtonRaster)
    _ui.colorLineEditRasterUpper->setColor(color);
  else if (_ui.buttonGroup->checkedButton() == _ui.radioButtonSign)
    _ui.colorLineEditSignsUpper->setColor(color);
  refreshPreviewImage();
}

void WinImageFilterGrid::on_buttonGroup_buttonClicked(QAbstractButton * button)
{
  auto enFilter = button != _ui.radioButtonOriginal;
  _ui.groupBoxLower->setEnabled(enFilter);
  _ui.groupBoxUpper->setEnabled(enFilter);

  if (button == _ui.radioButtonRaster)
  {
    _ui.widgetLowerFilter->setColor(_ui.colorLineEditRasterLower->color());
    _ui.widgetUpperFilter->setColor(_ui.colorLineEditRasterUpper->color());
  } else if (_ui.buttonGroup->checkedButton() == _ui.radioButtonSign)
  {
    _ui.widgetLowerFilter->setColor(_ui.colorLineEditSignsLower->color());
    _ui.widgetUpperFilter->setColor(_ui.colorLineEditSignsUpper->color());
  } else
  {
    QColor c = Qt::white;
    _ui.widgetLowerFilter->setColor(c);
    _ui.widgetUpperFilter->setColor(c);
  }
  refreshPreviewImage();
}

*/