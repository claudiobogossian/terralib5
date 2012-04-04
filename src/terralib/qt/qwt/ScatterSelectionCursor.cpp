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
