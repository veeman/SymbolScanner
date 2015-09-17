#include "WinSymbolScanner.h"
#include "QImageLoader.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QThreadPool>
#include <QDir>

WinSymbolScanner::WinSymbolScanner(QWidget *parent)
  : QMainWindow(parent)
{
  _ui.setupUi(this);

  // connect signals with pages
  qobject_cast<QMainWindowChild*>(_ui.pageSelect)->setParentMainWindow(this);
  qobject_cast<QMainWindowChild*>(_ui.pageFilter)->setParentMainWindow(this);

  QObject::connect(this, SIGNAL(imageCached(const QString&)),
                   _ui.pageSelect, SLOT(onImageAvailable(const QString&)));
  
  QObject::connect(_ui.pageSelect, SIGNAL(pathChanged(const QString&)),
                   _ui.pageFilter, SLOT(setCurrentFolder(const QString&)));

  // center widget and trigger current selected page as default
  setGeometry(QStyle::alignedRect(Qt::LeftToRight, 
                                  Qt::AlignCenter, size(), 
                                  qApp->desktop()->availableGeometry()));

  on_stackedWidget_currentChanged(_ui.stackedWidget->currentIndex());
}

WinSymbolScanner::~WinSymbolScanner()
{

}

void WinSymbolScanner::on_stackedWidget_currentChanged(int index)
{
  const QWidget *widget = _ui.stackedWidget->widget(index);
  const QString description = widget->property("description").toString();
  _ui.labelDescription->setText(description);

  const int lastIndex = _ui.stackedWidget->count() - 1;
  _ui.pushButtonNext->setVisible(index != lastIndex);
  _ui.pushButtonBack->setVisible((index != 0) && (index != lastIndex));
  _ui.pushButtonProcess->setVisible(index == lastIndex);
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
}

void WinSymbolScanner::on_pushButtonProcess_clicked(void)
{

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