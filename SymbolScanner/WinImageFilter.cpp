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

void WinImageFilter::reset(void)
{

}

void WinImageFilter::process(void)
{

}

void WinImageFilter::pageSelected(void)
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

    int filterMode = _ui.tabWidget->currentWidget()->property("id").toInt();

    bool autoRotate = filterMode == 0 ? _ui.checkBoxGridAutoRotate->isChecked() : false;
    bool invertedMask = filterMode == 0 ? _ui.checkBoxGridInvertMask->isChecked() : _ui.checkBoxSymbolInvertMask->isChecked();
    bool lineDetect = filterMode == 0;
    int filterPreviewType = (filterMode == 0 ? _ui.buttonGroupGridPreviewSelection : _ui.buttonGroupSymbolPreviewSelection)->checkedButton()->property("id").toInt();
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

    auto taskProcess = new QImageProcessor(image, autoRotate, invertedMask, lineDetect, previewType, filterUpperColor, filterLowerColor);
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

void WinImageFilter::on_tabWidget_currentChanged(int index)
{
  updateChanges([&](QImageFilterOptions& options)
  {
    _ui.widgetLowerColorSelector->blockSignals(true);
    _ui.widgetUpperColorSelector->blockSignals(true);

    if (_ui.tabWidget->currentWidget()->property("id").toInt() == 0)
    {
      _ui.widgetLowerColorSelector->setColor(options.gridLowerColor);
      _ui.widgetUpperColorSelector->setColor(options.gridUpperColor);
    } else
    {
      _ui.widgetLowerColorSelector->setColor(options.symbolLowerColor);
      _ui.widgetUpperColorSelector->setColor(options.symbolUpperColor);
    }

    _ui.widgetLowerColorSelector->blockSignals(false);
    _ui.widgetUpperColorSelector->blockSignals(false);
  });
}

void WinImageFilter::on_checkBoxGridAutoRotate_stateChanged(int state)
{
  updateChanges([state](QImageFilterOptions& options)
  {
    options.autoRotate = state == Qt::Checked;
  });
}

void WinImageFilter::on_checkBoxGridInvertMask_stateChanged(int state)
{
  updateChanges([state](QImageFilterOptions& options)
  {
    options.gridMaskInverted = state == Qt::Checked;
  });
}

void WinImageFilter::on_buttonGroupGridPreviewSelection_buttonClicked(QAbstractButton* button)
{
  updateChanges([](QImageFilterOptions&)
  {});
}

void WinImageFilter::on_checkBoxSymbolUseMask_stateChanged(int state)
{
  updateChanges([state](QImageFilterOptions& options)
  {
    options.symbolFilterEnabled = state == Qt::Checked;
  });
}

void WinImageFilter::on_checkBoxSymbolInvertMask_stateChanged(int state)
{
  updateChanges([state](QImageFilterOptions& options)
  {
    options.symbolMaskInverted = state == Qt::Checked;
  });
}

void WinImageFilter::on_buttonGroupSymbolPreviewSelection_buttonClicked(QAbstractButton* button)
{
  updateChanges([](QImageFilterOptions&)
  {});
}

void WinImageFilter::on_widgetUpperColorSelector_colorChanged(QColor color)
{
  updateChanges([&](QImageFilterOptions& options)
  {
    if (_ui.tabWidget->currentWidget()->property("id").toInt() == 0)
      options.gridUpperColor = color;
    else
      options.symbolUpperColor = color;
  });
}

void WinImageFilter::on_widgetLowerColorSelector_colorChanged(QColor color)
{
  updateChanges([&](QImageFilterOptions& options)
  {
    if (_ui.tabWidget->currentWidget()->property("id").toInt() == 0)
      options.gridLowerColor = color;
    else
      options.symbolLowerColor = color;
  });
}

void WinImageFilter::onc_listViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
  _currentFileName = _fileModel->filePath(current);
  updateChanges([&](QImageFilterOptions& options)
  {
    foreach(QWidget* w, findChildren<QWidget*>())
      w->blockSignals(true);

    _ui.checkBoxGridAutoRotate->setChecked(options.autoRotate);
    _ui.checkBoxGridInvertMask->setChecked(options.gridMaskInverted);
    _ui.checkBoxSymbolUseMask->setChecked(options.symbolFilterEnabled);
    _ui.checkBoxSymbolInvertMask->setChecked(options.symbolMaskInverted);

    if (_ui.tabWidget->currentWidget()->property("id").toInt() == 0)
    {
      _ui.widgetLowerColorSelector->setColor(options.gridLowerColor);
      _ui.widgetUpperColorSelector->setColor(options.gridUpperColor);
    } else
    {
      _ui.widgetLowerColorSelector->setColor(options.symbolLowerColor);
      _ui.widgetUpperColorSelector->setColor(options.symbolUpperColor);
    }

    foreach(QWidget* w, findChildren<QWidget*>())
      w->blockSignals(false);
  });
}

void WinImageFilter::setCurrentFolder(const QString& directory)
{
  _fileModel->setNameFilters(parentMainWindow()->property("defaultFileFilter").toStringList());
  _ui.listViewFiles->clearSelection();
  _ui.listViewFiles->setRootIndex(_fileModel->setRootPath(directory));

  updateChanges([](QImageFilterOptions&)
  {});
}

void WinImageFilter::updateChanges(std::function<void(QImageFilterOptions&)> callback)
{
  if (!_currentFileName.isEmpty() && callback)
  {
    auto& optionsCache = parentMainWindow()->imageFilterOptions();

    if (!optionsCache.contains(_currentFileName))
    {
      QImageFilterOptions newOptions;
      newOptions.autoRotate = _ui.checkBoxGridAutoRotate->isChecked();
      newOptions.gridMaskInverted = _ui.checkBoxGridInvertMask->isChecked();
      newOptions.symbolFilterEnabled = _ui.checkBoxSymbolUseMask->isChecked();
      newOptions.symbolMaskInverted = _ui.checkBoxSymbolInvertMask->isChecked();
      newOptions.gridLowerColor = _ui.widgetLowerColorSelector->property("color").value<QColor>();
      newOptions.gridUpperColor = _ui.widgetUpperColorSelector->property("color").value<QColor>();
      newOptions.symbolLowerColor = _ui.widgetLowerColorSelector->property("color").value<QColor>();
      newOptions.symbolUpperColor = _ui.widgetUpperColorSelector->property("color").value<QColor>();
      optionsCache[_currentFileName] = newOptions;
    }

    auto& options = optionsCache[_currentFileName];
    callback(options);
  }

  _configChanged = true;
  timerEvent(nullptr);
}