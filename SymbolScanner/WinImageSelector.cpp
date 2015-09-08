#include "WinImageSelector.h"
#include <QFileSystemModel>
#include <QDir>

WinImageSelector::WinImageSelector(QWidget *parent)
  : QWidget(parent), dirModel(nullptr), fileModel(nullptr)
{
  ui.setupUi(this);

  // setup directory listener
  dirModel = new QFileSystemModel(this);
  dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
  dirModel->setRootPath(dirModel->myComputer().toString());

  ui.treeViewDirs->setModel(dirModel);
  ui.treeViewDirs->hideColumn(3);
  ui.treeViewDirs->hideColumn(2);
  ui.treeViewDirs->hideColumn(1);

  connect(ui.treeViewDirs->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
          this, SLOT(onc_treeViewSelectionModel_currentChanged(const QModelIndex&, const QModelIndex&)));

  // setup file listener
  fileModel = new QFileSystemModel(this);
  dirModel->setRootPath(dirModel->myComputer().toString());
  fileModel->setFilter(QDir::Files);
  fileModel->setNameFilterDisables(false);
  fileModel->setReadOnly(false);

  ui.listViewFiles->setModel(fileModel);

  connect(ui.listViewFiles->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
          this, SLOT(onc_listViewSelectionModel_currentChanged(const QModelIndex&, const QModelIndex&)));
}

WinImageSelector::~WinImageSelector()
{
}

void WinImageSelector::onc_treeViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
  // selected directory changed, update file listener
  ui.listViewFiles->setRootIndex(fileModel->setRootPath(dirModel->filePath(current)));
  setSelectedFile();

  fileModel->setNameFilters(QStringList());
  fileModel->setNameFilters(QStringList() << "*.bmp" << "*.jpg" << "*.png");
}

void WinImageSelector::onc_listViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
  // selected file changed, update preview
  setSelectedFile(fileModel->filePath(current));
}

void WinImageSelector::setSelectedFile(const QString& fileName)
{
  if (fileName.isEmpty()) 
    return;

  QPixmap pix(fileName);
  ui.labelPreview->setPixmap(pix);
}
