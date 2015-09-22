#include "WinImageProcessor.h"
#include "WinSymbolScanner.h"
#include <QFileInfo>

WinImageProcessor::WinImageProcessor(QWidget *parent)
    : QMainWindowChild(parent),
    _processRunning(false)
{
  _ui.setupUi(this);

  _ui.treeWidget->setItemDelegateForColumn(0, new QNoEditDelegate());
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

    // TODO: create jobs
    auto& optionList = parentMainWindow()->imageFilterOptions();
    auto& imageCache = parentMainWindow()->imageCache();
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
    auto filePath = key;
    auto fInfo = QFileInfo(filePath);
    auto item = new QTreeWidgetItem(_ui.treeWidget, QStringList() << fInfo.fileName() << fInfo.baseName());
    item->setData(0, Qt::UserRole, filePath);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
  }
}
