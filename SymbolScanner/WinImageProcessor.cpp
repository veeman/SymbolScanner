#include "WinImageProcessor.h"
#include "WinSymbolScanner.h"
#include "QImageExtractor.h"
#include <QFileInfo>
#include <QThreadPool>
#include <QFileDialog>

WinImageProcessor::WinImageProcessor(QWidget *parent)
    : QMainWindowChild(parent),
    _processRunning(false)
{
  _ui.setupUi(this);

  auto delegate = new QNoEditDelegate();
  for (int i = 0; i < _ui.treeWidget->columnCount(); ++i)
  {
    _ui.treeWidget->resizeColumnToContents(i);
    if (i != 1)
      _ui.treeWidget->setItemDelegateForColumn(i, delegate);
  }

  _ui.lineEditOutput->setText(QDir::currentPath());
}

WinImageProcessor::~WinImageProcessor()
{

}

void WinImageProcessor::reset(void)
{
  _ui.treeWidget->clear();
}

void WinImageProcessor::pageSelected(void)
{
  reFillProcessList();

  quint32 navButtons = NAVBUTTONS_ALL & ~(NAVBUTTONS_NEXT);
  if (!_ui.treeWidget->topLevelItemCount())
    navButtons &= ~NAVBUTTONS_PROCESS;
  parentMainWindow()->setEnabledWizardButtons(navButtons);
}

void WinImageProcessor::process(void)
{
  // get needed process informations
  quint32 imOutputType = _ui.radioButtonMNIST->isChecked();
  QString imOutput(_ui.lineEditOutput->text());
  QSize imSize(_ui.spinBoxWidth->value(), _ui.spinBoxHeight->value());
  auto& imageProcessList = parentMainWindow()->imageFilterOptions();

  // clear cache to get more ram
  parentMainWindow()->imageCacheClear();

  // disable controls
  parentMainWindow()->setEnabledWizardButtons(0);
  _ui.pushButtonAbort->setEnabled(true);

  // prepare and run process
  _processRunning = true;
  _processHalt = false;
  auto taskProcess = new QImageExtractor(&_processHalt,
                                         imOutputType,
                                         imOutput,
                                         imSize,
                                         &imageProcessList);

  connect(taskProcess, SIGNAL(finished()), SLOT(onc_processThread_finished()));
  _ui.progressBar->connect(taskProcess, SIGNAL(progressDeterminated(int)), SLOT(setMaximum(int)));
  _ui.progressBar->connect(taskProcess, SIGNAL(progressChanged(int)), SLOT(setValue(int)));
  _ui.labelCurrentFile->connect(taskProcess, SIGNAL(progressMessage(const QString&)), SLOT(setText(const QString&)));
  
  QThreadPool::globalInstance()->start(taskProcess);
}

void WinImageProcessor::on_pushButtonAbort_clicked(void)
{
  _processHalt = true;
  _ui.pushButtonAbort->setEnabled(false);
}

void WinImageProcessor::on_radioButtonMNIST_clicked(void)
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), 
                                                  QDir::currentPath(),
                                                  tr("MNIST (*.-ubyte)"));
  _ui.lineEditOutput->setText(fileName);
}

void WinImageProcessor::on_radioButtonImage_clicked(void)
{
  QString cdir = _ui.lineEditOutput->text().isEmpty() ? QDir::currentPath() : _ui.lineEditOutput->text();
  QString dir = QFileDialog::getExistingDirectory(this, tr("Output Directory"),
                                                  cdir,
                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  _ui.lineEditOutput->setText(dir);
}

void WinImageProcessor::on_treeWidget_itemChanged(QTreeWidgetItem * item, int column)
{
  if (!item || !(column == 1))
      return;

  // check if only one leter is entered, else cut it to length 1
  QString text = item->text(1);
  if (text.length() > 1)
  {
    text = text.left(1);
    item->setText(1, text);
  }

  // update value in options field
  const auto filePath = item->data(0, Qt::UserRole).toString();
  auto& optionList = parentMainWindow()->imageFilterOptions();
  
  optionList[filePath].value = text;
}

void WinImageProcessor::onc_processThread_finished(void)
{
  parentMainWindow()->setEnabledWizardButtons(NAVBUTTONS_ALL);
}

void WinImageProcessor::reFillProcessList()
{
  _ui.treeWidget->clear();

  auto& optionList = parentMainWindow()->imageFilterOptions();

  auto keys = optionList.keys();
  qSort(keys.begin(), keys.end());

  for (auto &key: keys)
  {
    auto& options = optionList[key];
    auto filePath = key;
    auto fInfo = QFileInfo(filePath);

    if (options.value.isEmpty())
      options.value = fInfo.baseName();

    auto list = QStringList() << fInfo.fileName();
    list << options.value;
    list << QString("%0").arg(options.autoRotate);
    list << options.gridLowerColor.name();
    list << options.gridUpperColor.name();
    list << QString("%0").arg(options.gridMaskInverted);
    list << QString("%0").arg(options.symbolFilterEnabled);
    list << options.symbolLowerColor.name();
    list << options.symbolUpperColor.name();
    list << QString("%0").arg(options.symbolMaskInverted);

    auto item = new QTreeWidgetItem(_ui.treeWidget, list);
    item->setData(0, Qt::UserRole, filePath);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
  }

  _ui.treeWidget->resizeColumnToContents(0);
}
