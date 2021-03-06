#include "WinSymbolScanner.h"
#include "QImageLoader.h"
#include "QImageFilterOptions.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QThreadPool>
#include <QDir>

WinSymbolScanner::WinSymbolScanner(QWidget *parent)
  : QMainWindow(parent)
{
  _ui.setupUi(this);

  // connect signals with pages
  for (int i = 0; i<_ui.stackedWidget->count(); ++i)
  {
    QMainWindowChild *widget = qobject_cast<QMainWindowChild*>(_ui.stackedWidget->widget(i));
    if (widget)
      widget->setParentMainWindow(this);
  }

  QObject::connect(this, SIGNAL(imageCached(const QString&)),
                   _ui.pageSelect, SLOT(onImageAvailable(const QString&)));
  
  QObject::connect(_ui.pageSelect, SIGNAL(pathChanged(const QString&)),
                   _ui.pageFilter, SLOT(setCurrentFolder(const QString&)));

  // center widget and trigger current selected page as default
  setGeometry(QStyle::alignedRect(Qt::LeftToRight, 
                                  Qt::AlignCenter, size(), 
                                  qApp->desktop()->availableGeometry()));

  on_stackedWidget_currentChanged(_ui.stackedWidget->currentIndex());
  on_pushButtonRestart_clicked();
}

WinSymbolScanner::~WinSymbolScanner()
{

}

void WinSymbolScanner::setEnabledWizardButtons(quint32 navButtons)
{
  _ui.pushButtonBack->setEnabled(navButtons & NAVBUTTONS_BACK);
  _ui.pushButtonNext->setEnabled(navButtons & NAVBUTTONS_NEXT);
  _ui.pushButtonProcess->setEnabled(navButtons & NAVBUTTONS_PROCESS);
  _ui.pushButtonRestart->setEnabled(navButtons & NAVBUTTONS_RESTART);
}

void WinSymbolScanner::on_stackedWidget_currentChanged(int index)
{
  QWidget* widget = _ui.stackedWidget->widget(index);
  const QString description = widget->property("description").toString();
  const quint32 navButtons = widget->property("navButtons").toUInt();

  _ui.labelDescription->setText(description);
  setEnabledWizardButtons(NAVBUTTONS_ALL);

  _ui.pushButtonBack->setVisible(navButtons & NAVBUTTONS_BACK);
  _ui.pushButtonNext->setVisible(navButtons & NAVBUTTONS_NEXT);
  _ui.pushButtonProcess->setVisible(navButtons & NAVBUTTONS_PROCESS);
  _ui.pushButtonRestart->setVisible(navButtons & NAVBUTTONS_RESTART);

  QMainWindowChild *child = qobject_cast<QMainWindowChild*>(widget);
  if (child)
    child->pageSelected();
}

void WinSymbolScanner::on_pushButtonNext_clicked(void)
{
  int idx = std::min(_ui.stackedWidget->currentIndex() + 1, _ui.stackedWidget->count() - 1);
  _ui.stackedWidget->setCurrentIndex(idx);
}

void WinSymbolScanner::on_pushButtonBack_clicked(void)
{
  int idx = std::max(_ui.stackedWidget->currentIndex() - 1, 0);
  _ui.stackedWidget->setCurrentIndex(idx);
}

void WinSymbolScanner::on_pushButtonRestart_clicked(void)
{
  for (int i = 0; i<_ui.stackedWidget->count(); ++i)
  {
    QMainWindowChild *widget = qobject_cast<QMainWindowChild*>(_ui.stackedWidget->widget(i));
    if (widget)
      widget->reset();
  }

  _ui.stackedWidget->setCurrentIndex(0);

  _imageCache.clear();
  _imageFilterOptions.clear();
}

void WinSymbolScanner::on_pushButtonProcess_clicked(void)
{
  QMainWindowChild *widget = qobject_cast<QMainWindowChild*>(_ui.stackedWidget->currentWidget());
  if (widget)
    widget->process();
}

void WinSymbolScanner::recacheImages(const QString& directory)
{
  //TODO: remove obsolute images
  //_imageCache.clear();

  auto fileList = QDir(directory).entryInfoList(property("defaultFileFilter").toStringList(),
                                                QDir::Files | QDir::NoDotAndDotDot);

  for (auto file : fileList)
  {
    auto filePath = file.filePath();
    if (_imageCache.find(filePath) != _imageCache.end())
      continue;

    auto taskLoad = new QImageLoader(filePath);
    connect(taskLoad, SIGNAL(finished(const QString&, const QImage&)), 
            this, SLOT(imageFileLoaded(const QString&, const QImage&)));
    QThreadPool::globalInstance()->start(taskLoad);
  }
}

void WinSymbolScanner::imageFileLoaded(const QString& fileName, const QImage& image)
{
  _imageCache.insert(fileName, QPixmap::fromImage(image));
  emit imageCached(fileName);
}