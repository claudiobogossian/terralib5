// TerraLib
#include "GoesItem.h"
#include "AnimationScene.h"
#include "Animation.h"
#include "../canvas/MapDisplay.h"
#include "../canvas/Canvas.h"

// Qt
#include <QtCore/QPropertyAnimation>
#include <QtGui/QPainter>
#include <QtCore/QVector>

te::qt::widgets::GoesItem::GoesItem()
  : te::qt::widgets::ImageItem()
{
}

te::qt::widgets::GoesItem::GoesItem(const QString& title, const QString& file, te::qt::widgets::MapDisplay* display)
  : te::qt::widgets::ImageItem(title, file, display)
{
}

te::qt::widgets::GoesItem::~GoesItem()
{
}

te::dt::TimeInstant te::qt::widgets::GoesItem::getTime(QString fileName)
{
  QString file(fileName);
  int ind = file.indexOf("_");
  if (file.indexOf("_", ind + 1) != -1)
    ind = file.indexOf("_", ind + 1);
  file.remove(0, ind + 1);
  QString ano(file), mes(file), dia(file), hour(file), min(file);
  ano.remove(4, ano.length() - 4);
  mes.remove(0, 4);
  mes.remove(2, mes.length() - 2);
  dia.remove(0, 6);
  dia.remove(2, dia.length() - 2);
  hour.remove(0, 8);
  hour.remove(2, hour.length() - 2);
  min.remove(0, 10);
  min.remove(2, min.length() - 2);

  te::dt::Date date(ano.toInt(), mes.toInt(), dia.toInt());
  te::dt::TimeDuration dur(hour.toInt(), min.toInt(), 0);
  return te::dt::TimeInstant(date, dur);
}

