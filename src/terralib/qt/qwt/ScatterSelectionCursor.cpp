#include "ScatterSelectionCursor.h"

//QWT
#include <qwt_picker_machine.h>

//QT
#include <QPainter>
#include <QWheelEvent>

te::qt::qwt::ScatterSelectionCursor::ScatterSelectionCursor(QWidget *parent) :
  QwtPicker(parent)
{
}

te::qt::qwt::ScatterSelectionCursor::ScatterSelectionCursor(RubberBand rubberBand, DisplayMode trackerMode, QWidget* parent) :
  QwtPicker(rubberBand, trackerMode, parent)
{
}

void te::qt::qwt::ScatterSelectionCursor::widgetMouseReleaseEvent(QMouseEvent* e)
{
  QwtPicker::widgetMouseReleaseEvent(e);

  //QMouseEvent* m = new QMouseEvent(QEvent::MouseButtonPress, e->pos(), e->button(), e->buttons(), e->modifiers());
  //QwtPicker::widgetMousePressEvent(m);
  QMouseEvent m(QEvent::MouseButtonPress, e->pos(), e->button(), e->buttons(), e->modifiers());
  QwtPicker::widgetMousePressEvent(&m);
  QwtPicker::widgetMouseReleaseEvent(e);
}

void te::qt::qwt::ScatterSelectionCursor::drawRubberBand(QPainter* painter) const
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
    QRect rect(p0, p1);
    painter->drawRect(rect);
  }
}

bool te::qt::qwt::ScatterSelectionCursor::end(bool ok)
{
  bool bok = QwtPicker::end(ok);
  if ( !ok )
    return false;

  const QPolygon &pa = selection();
  if ( pa.count() < 2 )
    return false;

  QRect rect = QRect( pa[0], pa[int( pa.count() - 1 )] );
  rect = rect.normalized();

  Q_EMIT select(rect);
  return bok;
}
