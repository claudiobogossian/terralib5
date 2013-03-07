#include "Legend.h"
#include "Histogram.h"

//QT
#include <QMouseEvent>
#include <QWidget>

te::qt::qwt::Legend::Legend(QWidget *parent) :
  QwtLegend(parent)
{
}

bool te::qt::qwt::Legend::eventFilter(QObject *ob, QEvent *e)
{
  QEvent::Type type = e->type();
  if(type == QEvent::MouseButtonPress)
  {
    QMouseEvent* m = (QMouseEvent*)e;
    if(m->button() == Qt::RightButton)
    {
      QPoint mp = m->pos();
      QList<QWidget*> wList =	legendItems();
      QList<QWidget*>::iterator it;
      for(it = wList.begin(); it != wList.end(); ++it)
      {
        QWidget* w = *it;
        QRect rec = w->rect();
        QPoint p = w->pos();
        rec.moveTopLeft(p);
        if(rec.contains(mp))
        {
          QPoint globalPoint = m->globalPos();
          emit legendContextMenu(globalPoint, w);
          break;
        }
      }
    }
  }
  return QwtLegend::eventFilter(ob, e);
}
