#ifndef __RENDER_AREA_H_
#define __RENDER_AREA_H_

#include <QWidget.h>
#include <QPixmap.h>

class RenderArea : public QWidget
{
public:
  RenderArea(QWidget *parent = 0);
  ~RenderArea();

  QSize minimumSizeHint() const;

protected:
  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *event);

public:
  QPixmap* m_pixmap;
};
#endif
