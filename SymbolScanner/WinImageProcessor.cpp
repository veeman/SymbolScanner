#include "WinImageProcessor.h"
#include "WinSymbolScanner.h"
#include <QFileInfo>

WinImageProcessor::WinImageProcessor(QWidget *parent)
    : QMainWindowChild(parent)
{
  _ui.setupUi(this);

  _ui.treeWidget->setItemDelegateForColumn(1, new QNoEditDelegate());
}

WinImageProcessor::~WinImageProcessor()
{

}

void WinImageProcessor::reset(void)
{
  _ui.treeWidget->clear();
}

void WinImageProcessor::reFillProcessList()
{
  _ui.treeWidget->clear();

  auto& optionList = parentMainWindow()->imageFilterOptions();

  auto element = optionList.constBegin();
  while (element != optionList.constEnd())
  {
    auto filePath = element.key();
    auto fInfo = QFileInfo(filePath);
    auto item = new QTreeWidgetItem(_ui.treeWidget, QStringList() << fInfo.fileName() << fInfo.baseName());
    item->setData(0, Qt::UserRole, filePath);
  }
}
