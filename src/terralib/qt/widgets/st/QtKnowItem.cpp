// TerraLib
#include "QtKnowItem.h"
#include "AnimationScene.h"
#include "Animation.h"
#include "../canvas/MapDisplay.h"
#include "../canvas/Canvas.h"

// Qt
#include <QtCore/QPropertyAnimation>
#include <QtGui/QPainter>
#include <QtCore/QVector>

te::qt::widgets::QtKnowItem::QtKnowItem()
  : te::qt::widgets::ImageItem()
{
}

te::qt::widgets::QtKnowItem::QtKnowItem(const QString& title, const QString& file, te::qt::widgets::MapDisplay* display)
  : te::qt::widgets::ImageItem(title, file, display)
{
}

te::qt::widgets::QtKnowItem::~QtKnowItem()
{
}

te::dt::TimeInstant te::qt::widgets::QtKnowItem::getTime(QString fileName)
{
  QString file(fileName);
  int ind = file.indexOf("_") + (int)strlen("_");
  file.remove(0, ind);
  QString ano = file.mid(0, 4);
  QString mes = file.mid(5, 2);
  QString dia = file.mid(8, 2);
  QString hour = file.mid(11, 2);
  QString min = file.mid(14, 2);
  QString sec = file.mid(17, 2);

  te::dt::Date date(ano.toInt(), mes.toInt(), dia.toInt());
  te::dt::TimeDuration dur(hour.toInt(), min.toInt(), sec.toInt());
  return te::dt::TimeInstant(date, dur);
}
