#include "WinImageProcessor.h"
#include "WinSymbolScanner.h"
#include <QFileInfo>

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
  if (_processRunning)
  {

  }
  else
  {
    parentMainWindow()->setEnabledWizardButtons(0);
    parentMainWindow()->imageCacheClear();

    _processRunning = true;

    auto& imageProcessList = parentMainWindow()->imageFilterOptions();
  }
}

void WinImageProcessor::reFillProcessList()
{
  _ui.treeWidget->clear();

  auto& optionList = parentMainWindow()->imageFilterOptions();

  auto keys = optionList.keys();
  qSort(keys.begin(), keys.end());

  for (auto &key: keys)
  {
    auto options = optionList[key];
    auto filePath = key;
    auto fInfo = QFileInfo(filePath);
    auto list = QStringList() << fInfo.fileName() << fInfo.baseName();
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
