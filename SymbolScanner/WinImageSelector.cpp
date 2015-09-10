#include "WinImageSelector.h"
#include <QThreadPool>
#include <QFileSystemModel>
#include <QDir>



// **********************************************************************************

WinImageSelector::WinImageSelector(QWidget *parent)
  : QWidget(parent), _dirModel(nullptr), _fileModel(nullptr), _currentDirectory()
{
  _ui.setupUi(this);

  // setup directory listener
  _dirModel = new QFileSystemModel(this);
  _dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
  _dirModel->setRootPath(_dirModel->myComputer().toString());

  _ui.treeViewDirs->setModel(_dirModel);
  _ui.treeViewDirs->hideColumn(3);
  _ui.treeViewDirs->hideColumn(2);
  _ui.treeViewDirs->hideColumn(1);

  connect(_ui.treeViewDirs->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
          this, SLOT(onc_treeViewSelectionModel_currentChanged(const QModelIndex&, const QModelIndex&)));

  // setup file listener
  _fileModel = new QFileSystemModel(this);
  _dirModel->setRootPath(_dirModel->myComputer().toString());
  _fileModel->setFilter(QDir::Files);
  _fileModel->setNameFilterDisables(false);
  _fileModel->setReadOnly(false);

  _ui.listViewFiles->setModel(_fileModel);

  connect(_ui.listViewFiles->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
          this, SLOT(onc_listViewSelectionModel_currentChanged(const QModelIndex&, const QModelIndex&)));
}

WinImageSelector::~WinImageSelector()
{}

void WinImageSelector::onc_treeViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
  // selected directory changed, update file listener
  _currentDirectory = _dirModel->filePath(current);
  _ui.listViewFiles->setRootIndex(_fileModel->setRootPath(_currentDirectory));
  recacheImages(_currentDirectory);

  _fileModel->setNameFilters(QStringList());
  _fileModel->setNameFilters(_ui.listViewFiles->property("defaultFilter").toStringList());
}

void WinImageSelector::onc_listViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
  // selected file changed, update preview
  setSelectedFile(_fileModel->filePath(current));
}

void WinImageSelector::setSelectedFile(const QString& fileName)
{
  _currentFile = fileName;

  if (fileName.isEmpty())
  {
    _ui.labelPreview->setPixmap(QPixmap());
    return;
  }

  _ui.labelPreview->setPixmap(_imageCache.value(fileName, QPixmap()));
}

void WinImageSelector::fileLoaded(const QString& fileName, const QImage& image)
{
  _imageCache.insert(fileName, QPixmap::fromImage(image));

  if (_currentFile == fileName)
    setSelectedFile(fileName);
}

void WinImageSelector::recacheImages(const QString& directory)
{
  _imageCache.clear();

  auto fileList = QDir(directory).entryInfoList(_ui.listViewFiles->property("defaultFilter").toStringList(), 
                                                QDir::Files | QDir::NoDotAndDotDot);

  for (auto file : fileList)
  {
    auto taskLoad = new QImageLoader(file.filePath());
    connect(taskLoad, SIGNAL(finished(const QString&, const QImage&)), this, SLOT(fileLoaded(const QString&, const QImage&)));
    QThreadPool::globalInstance()->start(taskLoad);
  }

  setSelectedFile();
}