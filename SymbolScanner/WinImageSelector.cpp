#include "WinImageSelector.h"
#include <QFileSystemModel>
#include <QDir>

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
  setSelectedFile();

  _fileModel->setNameFilters(QStringList());
  _fileModel->setNameFilters(QStringList() << "*.bmp" << "*.jpg" << "*.png");
}

void WinImageSelector::onc_listViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
  // selected file changed, update preview
  setSelectedFile(_fileModel->filePath(current));
}

void WinImageSelector::setSelectedFile(const QString& fileName)
{
  if (fileName.isEmpty())
    return;

  QPixmap pix(fileName);
  _ui.labelPreview->setPixmap(pix);
}
