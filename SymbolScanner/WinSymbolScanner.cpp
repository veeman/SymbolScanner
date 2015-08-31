#include "WinSymbolScanner.h"
#include <QGraphicsScene>
#include <QFileSystemModel>
#include <QDir>
#include <QShowEvent>

WinSymbolScanner::WinSymbolScanner(QWidget *parent)
  : QMainWindow(parent), dirModel(nullptr), fileModel(nullptr), previewScene(nullptr)
{
  ui.setupUi(this);

  // setup image preview
  previewScene = new QGraphicsScene(ui.graphicsViewPreview);
  ui.graphicsViewPreview->setScene(previewScene);
  ui.graphicsViewPreview->installEventFilter(this);

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

WinSymbolScanner::~WinSymbolScanner()
{

}

bool WinSymbolScanner::eventFilter(QObject* object, QEvent* event)
{
  // fit preview image in widget on resize event
  if (object == ui.graphicsViewPreview && (event->type() == QEvent::Resize || event->type() == QEvent::Show))
  {
    ui.graphicsViewPreview->fitInView(previewScene->sceneRect(), Qt::KeepAspectRatio);
    return true;
  }

  return false;
}

void WinSymbolScanner::showEvent(QShowEvent * event)
{
  eventFilter(ui.graphicsViewPreview, event);
}

void WinSymbolScanner::onc_treeViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
  // selected directory changed, update file listener
  ui.listViewFiles->setRootIndex(fileModel->setRootPath(dirModel->filePath(current)));
  setSelectedFile();

  fileModel->setNameFilters(QStringList());
  fileModel->setNameFilters(QStringList() << "*.bmp" << "*.jpg" << "*.png");
}

void WinSymbolScanner::onc_listViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
  // selected file changed, update preview
  setSelectedFile(fileModel->filePath(current));
}

void WinSymbolScanner::setSelectedFile(const QString& fileName)
{
  setWindowTitle(fileName);

  if (!previewScene) return;
  previewScene->clear();

  if (fileName.isEmpty()) return;
  QPixmap pix(fileName);
  previewScene->addPixmap(pix);
}