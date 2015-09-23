#include "ColorMapSliceItem.h"

// Qt
#include <QPainter>

QIcon GetIcon(const QColor& beg, const QColor& end)
{
  if(!end.isValid())
  {
    QPixmap pix(16, 16);
    pix.fill(beg);
    return QIcon(pix);
  }

  QPixmap pix(16, 16);
  QPainter p(&pix);

  p.fillRect(0, 0, 8, 16, beg);
  p.fillRect(8, 0, 8, 16, end);

  return QIcon(pix);
}

te::qt::widgets::ColorMapSliceItem::ColorMapSliceItem(const std::string& label, const QColor& beg, const QColor& end):
TreeItem("COLORMAPSLICE"),
m_label(label)
{
  m_icon = GetIcon(beg, end);
}

te::qt::widgets::ColorMapSliceItem::~ColorMapSliceItem()
{
}

std::string te::qt::widgets::ColorMapSliceItem::getAsString() const
{
  return m_label;
}

QIcon te::qt::widgets::ColorMapSliceItem::getIcon() const
{
  return m_icon;
}
