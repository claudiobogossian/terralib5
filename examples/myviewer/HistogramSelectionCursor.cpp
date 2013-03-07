#include "HistogramSelectionCursor.h"

//QWT
#include <qwt_picker_machine.h>

//QT
#include <QPainter>
#include <QWheelEvent>

te::qt::qwt::HistogramSeletionCursor::HistogramSeletionCursor(QWidget *parent) :
  QwtPicker(parent)
{
}

te::qt::qwt::HistogramSeletionCursor::HistogramSeletionCursor(RubberBand rubberBand, DisplayMode trackerMode, QWidget* parent) :
  QwtPicker(rubberBand, trackerMode, parent)
{
}

void te::qt::qwt::HistogramSeletionCursor::widgetMouseReleaseEvent(QMouseEvent* e)
{
  QwtPicker::widgetMouseReleaseEvent(e);

  //QMouseEvent* m = new QMouseEvent(QEvent::MouseButtonPress, e->pos(), e->button(), e->buttons(), e->modifiers());
  //QwtPicker::widgetMousePressEvent(m);
  QMouseEvent m(QEvent::MouseButtonPress, e->pos(), e->button(), e->buttons(), e->modifiers());
  QwtPicker::widgetMousePressEvent(&m);
  QwtPicker::widgetMouseReleaseEvent(e);
}

void te::qt::qwt::HistogramSeletionCursor::drawRubberBand(QPainter* painter) const
{
  if ( !isActive() || rubberBand() == NoRubberBand ||
    rubberBandPen().style() == Qt::NoPen )
  {
    return;
  }

  const QPolygon pa = adjustedPoints(pickedPoints());

  QwtPickerMachine::SelectionType selectionType = QwtPickerMachine::NoSelection;

  if (stateMachine() )
    selectionType = stateMachine()->selectionType();

  if(selectionType == QwtPickerMachine::PolygonSelection)
  {
    if(pa.count() < 2)
      return;
    int h = 4;
    QPoint p0 = pa.point(0);
    QPoint p1 = pa.point(1);
    p1.setY(p0.y());

    painter->drawLine(p0, p1);
    painter->drawLine(p0.x(), p0.y()-h/2, p0.x(), p0.y()+h/2);
    painter->drawLine(p1.x(), p1.y()-h/2, p1.x(), p1.y()+h/2);  
  }
}

bool te::qt::qwt::HistogramSeletionCursor::end(bool ok)
{
  bool bok = QwtPicker::end(ok);
  if(bok)
  {
    QPolygon pol = adjustedPoints(pickedPoints());
    QPoint p0 = pol.point(0);
    QPoint p1 = pol.point(1);

    Q_EMIT select(p0.x(), p1.x());
  }
  return bok;
}

bool te::qt::qwt::HistogramSeletionCursor::eventFilter(QObject* ob, QEvent* e)
{
  QEvent::Type type = e->type();
  if(type == QEvent::Wheel)
  {
    QWheelEvent* wheel = (QWheelEvent*)e;
    int delta = wheel->delta();
    emit wheelEvent(delta);
  }
  return QwtPicker::eventFilter(ob, e);
}
