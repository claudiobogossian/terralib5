#include "ChartSliceItem.h"

// Qt
#include <QObject>
#include <QPainter>

QIcon CreateIcon(const int& size, const QColor& penColor, const QColor& brushColor, const int& contourSize)
{
  QPixmap pix(size, size);
  pix.fill(Qt::transparent);

  int offset = 2;

  QPainterPath path;
  path.addRect(offset, offset, pix.width() - 2 * offset, pix.height() - 2 * offset);

  QPen pen;
  pen.setColor(penColor);
  pen.setWidth(contourSize);

  QBrush brush;
  brush.setColor(brushColor);

  QPainter p(&pix);
  p.setPen(pen);
  p.setBrush(brushColor);

  p.fillPath(path, brush);
  p.drawPath(path);

  return QIcon(pix);
}


te::qt::widgets::ChartSliceItem::ChartSliceItem(const QString& propertyName, const QColor& color,
                                                const QColor& contourColor, const int& contourWidth):
  TreeItem("CHARTSLICE")
{
  m_label = (QObject::tr("Property") + ": " + propertyName).toStdString();

  m_icon = CreateIcon(16, contourColor, color, contourWidth);
}

te::qt::widgets::ChartSliceItem::~ChartSliceItem()
{
}

std::string te::qt::widgets::ChartSliceItem::getAsString() const
{
  return m_label;
}

QIcon te::qt::widgets::ChartSliceItem::getIcon() const
{
  return m_icon;
}
