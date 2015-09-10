#include "QAspectRatioPixmapLabel.h"

QAspectRatioPixmapLabel::QAspectRatioPixmapLabel(QWidget *parent) :
  QLabel(parent)
{
  this->setMinimumSize(1, 1);
  this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

void QAspectRatioPixmapLabel::setPixmap(const QPixmap & p)
{
  pix = p;
  resizeEvent(0);
}

int QAspectRatioPixmapLabel::heightForWidth(int width) const
{
  return ((qreal)pix.height()*width) / pix.width();
}

QSize QAspectRatioPixmapLabel::sizeHint() const
{
  int w = this->width();
  return QSize(w, heightForWidth(w));
}

void QAspectRatioPixmapLabel::resizeEvent(QResizeEvent * e)
{
  if (!pix.isNull())
    QLabel::setPixmap(pix.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}