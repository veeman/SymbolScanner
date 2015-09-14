#include "WinImageFilter.h"
#include "WinSymbolScanner.h"
#include "QImageProcessor.h"
#include <QFileSystemModel>
#include <QThreadPool>

WinImageFilter::WinImageFilter(QWidget *parent)
  : QMainWindowChild(parent),
  _fileModel(nullptr),
  _configChanged(false),
  _processRunning(false),
  _currentFileName(QString())
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

  startTimer(1000);
}

WinImageFilter::~WinImageFilter()
{

}

void WinImageFilter::timerEvent(QTimerEvent * event)
{
  if (_configChanged && !_processRunning)
  {
    if (_currentFileName.isNull())
    {
      _ui.labelPreview->setText(_ui.labelPreview->property("defaultText").toString());
      _configChanged = false;
      return;
    }

    QImage image = parentMainWindow()->imageCache().value(_currentFileName).toImage();
    bool autoRotate = (_ui.checkBoxGridAutoRotate->checkState() == Qt::Checked);
    bool invertedMask = (_ui.checkBoxGridInvertMask->checkState() == Qt::Checked);
    int filterPreviewType = _ui.buttonGroupGridPreviewSelection->checkedButton()->property("id").toInt();
    QColor filterUpperColor = _ui.widgetUpperColorSelector->color();
    QColor filterLowerColor = _ui.widgetLowerColorSelector->color();

    QImageProcessor::FilterType previewType;
    switch (filterPreviewType)
    {
      case 1:
        previewType = QImageProcessor::Mask;
        break;
      case 2:
        previewType = QImageProcessor::AppliedMask;
          break;
      default:
        previewType = QImageProcessor::None;
        break;
    }

    _configChanged = false;
    _processRunning = true;

    auto taskProcess = new QImageProcessor(image, autoRotate, invertedMask, previewType, filterUpperColor, filterLowerColor);
    connect(taskProcess, SIGNAL(finished(const QImage&)),
            this, SLOT(refreshPreview(const QImage&)));
    QThreadPool::globalInstance()->start(taskProcess);
  }
}

void WinImageFilter::refreshPreview(const QImage& image)
{
  _processRunning = false;
  _ui.labelPreview->setPixmap(QPixmap::fromImage(image));
}

void WinImageFilter::on_checkBoxGridAutoRotate_stateChanged(int state)
{
  _configChanged = true;
  timerEvent(nullptr);
}

void WinImageFilter::on_checkBoxGridInvertMask_stateChanged(int state)
{
  _configChanged = true;
  timerEvent(nullptr);
}

void WinImageFilter::on_buttonGroupGridPreviewSelection_buttonClicked(QAbstractButton* button)
{
  _configChanged = true;
  timerEvent(nullptr);
}

void WinImageFilter::on_widgetUpperColorSelector_colorChanged(QColor color)
{
  _configChanged = true;
  timerEvent(nullptr);
}

void WinImageFilter::on_widgetLowerColorSelector_colorChanged(QColor color)
{
  _configChanged = true;
  timerEvent(nullptr);
}

void WinImageFilter::onc_listViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
  _currentFileName = _fileModel->filePath(current);
  _configChanged = true;
  timerEvent(nullptr);
}

void WinImageFilter::setCurrentFolder(const QString& directory)
{
  _fileModel->setNameFilters(parentMainWindow()->property("defaultFileFilter").toStringList());
  _ui.listViewFiles->clearSelection();
  _ui.listViewFiles->setRootIndex(_fileModel->setRootPath(directory));

  _currentFileName = QString();
  _configChanged = true;
  timerEvent(nullptr);
}