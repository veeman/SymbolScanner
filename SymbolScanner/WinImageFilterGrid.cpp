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

void WinImageFilterGrid::refreshPreview(void)
{
  // _currentFileName, _autoRotate, _filterPreviewType, _filterUpperColor, _filterLowerColor

  /*
  QPixmap orgPix = parentMainWindow()->imageCache().value(fileName);
  cv::Mat org = qPixmapToCvMat(orgPix, false);

  cv::Mat greyMat;
  cv::cvtColor(org, greyMat, CV_BGR2GRAY);

  QPixmap editedPix = cvMatToQPixmap(greyMat);
  _ui.labelPreview->setPixmap(editedPix);
  */
}

void WinImageFilterGrid::on_checkBoxAutoRotate_stateChanged(int state)
{
  _autoRotate = (state == Qt::Checked);
  refreshPreview();
}

void WinImageFilterGrid::on_buttonGroupPreviewSelection_buttonClicked(QAbstractButton* button)
{
  _filterPreviewType = button->property("id").toInt();
  refreshPreview();
}

void WinImageFilterGrid::on_widgetUpperColorSelector_colorChanged(QColor color)
{
  _filterUpperColor = color;
  refreshPreview();
}

void WinImageFilterGrid::on_widgetLowerColorSelector_colorChanged(QColor color)
{
  _filterLowerColor = color;
  refreshPreview();
}

void WinImageFilterGrid::onc_listViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
  _currentFileName = _fileModel->filePath(current);
  refreshPreview();
}

void WinImageFilterGrid::setCurrentFolder(const QString& directory)
{
  _fileModel->setNameFilters(parentMainWindow()->property("defaultFileFilter").toStringList());
  _ui.listViewFiles->clearSelection();
  _ui.listViewFiles->setRootIndex(_fileModel->setRootPath(directory));
}