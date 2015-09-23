#ifndef WINIMAGEFILTEROPTIONS_H
#define WINIMAGEFILTEROPTIONS_H

#include "QMainWindowChild.h"
#include "ui_WinImageFilter.h"
#include "QImageFilterOptions.h"
#include <functional>

class QListWidgetItem;
class QFileSystemModel;
class QTimerEvent;

class WinImageFilter : public QMainWindowChild
{
  Q_OBJECT

public:

  WinImageFilter(QWidget *parent = 0);
  ~WinImageFilter();

  virtual void reset(void) override;
  virtual void process(void) override;
  virtual void pageSelected(void) override;

protected:
  virtual void timerEvent(QTimerEvent * event);

  public slots:
  void refreshPreview(const QImage& image);

  void on_tabWidget_currentChanged(int index);

  void on_checkBoxGridAutoRotate_stateChanged(int state);
  void on_checkBoxGridInvertMask_stateChanged(int state);
  void on_buttonGroupGridPreviewSelection_buttonClicked(QAbstractButton* button);

  void on_checkBoxSymbolUseMask_stateChanged(int state);
  void on_checkBoxSymbolInvertMask_stateChanged(int state);
  void on_buttonGroupSymbolPreviewSelection_buttonClicked(QAbstractButton* button);

  void on_widgetUpperColorSelector_colorChanged(QColor color);
  void on_widgetLowerColorSelector_colorChanged(QColor color);

  void onc_listViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous);
  void setCurrentFolder(const QString& directory);
  void updateChanges(std::function<void(QImageFilterOptions&)> callback);

private:
  Ui::WinImageFilter _ui;
  QFileSystemModel *_fileModel;
  
  bool _configChanged;
  bool _processRunning;
  QString _currentFileName;
};

#endif // WINIMAGEFILTEROPTIONS_H
