#include "WinImageSelector.h"
#include "WinSymbolScanner.h"
#include <QFileSystemModel>

WinImageSelector::WinImageSelector(QWidget *parent)
  : QMainWindowChild(parent), _dirModel(nullptr), _fileModel(nullptr), _currentDirectory()
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
  _fileModel->setFilter(QDir::Files);
  _fileModel->setNameFilterDisables(false);
  _fileModel->setReadOnly(false);

  _ui.listViewFiles->setModel(_fileModel);

  connect(_ui.listViewFiles->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
          this, SLOT(onc_listViewSelectionModel_currentChanged(const QModelIndex&, const QModelIndex&)));
}

WinImageSelector::~WinImageSelector()
{}

void WinImageSelector::reset(void)
{
  _ui.treeViewDirs->setCurrentIndex(_dirModel->index(QDir::currentPath()));
}

void WinImageSelector::onc_treeViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
  // selected directory changed, update file listener
  _currentDirectory = _dirModel->filePath(current);
  _ui.listViewFiles->setRootIndex(_fileModel->setRootPath(_currentDirectory));
  setSelectedFile();

  emit pathChanged(_currentDirectory);

  if (!parentMainWindow())
    return;

  parentMainWindow()->recacheImages(_currentDirectory);

  _fileModel->setNameFilters(QStringList());
  _fileModel->setNameFilters(parentMainWindow()->property("defaultFileFilter").toStringList());
}

void WinImageSelector::onc_listViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
  // selected file changed, update preview
  setSelectedFile(_fileModel->filePath(current));
}

void WinImageSelector::onImageAvailable(const QString& fileName)
{
  if (fileName == _currentFile)
    setSelectedFile(fileName);
}

void WinImageSelector::setSelectedFile(const QString& fileName)
{
  _currentFile = fileName;

  if (fileName.isEmpty())
  {
    _ui.listViewFiles->clearSelection();
    _ui.labelPreview->setText(_ui.labelPreview->property("defaultText").toString());
    return;
  }

  QPixmap pix; 

  if (parentMainWindow())
    pix = parentMainWindow()->imageCache().value(fileName, QPixmap());

  if (pix.isNull())
    _ui.labelPreview->setText(_ui.labelPreview->property("loadingText").toString());
  else
    _ui.labelPreview->setPixmap(pix);
}