#ifndef QASPECTRATIOPIXMAPLABEL_H
#define QASPECTRATIOPIXMAPLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

/**
  * phyatt 2014
  * http://stackoverflow.com/a/22618496
  */
class QAspectRatioPixmapLabel : public QLabel
{
  Q_OBJECT
public:
  explicit QAspectRatioPixmapLabel(QWidget *parent = 0);
  virtual int heightForWidth(int width) const;
  virtual QSize sizeHint() const;

  public slots :
  void setPixmap(const QPixmap &);
  void resizeEvent(QResizeEvent *);

private:
  QPixmap pix;
};

#endif // QASPECTRATIOPIXMAPLABEL_H