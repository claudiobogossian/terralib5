#include "StarDelegate.h"


//! Qt include files
#include <QPainter>

//! STL include files
#include <cmath>

QPainterPath getStar()
{
  QPainterPath star, aux;

  aux.setFillRule(Qt::WindingFill);

  aux.moveTo(1.0, 0.5);
  for(int i = 1; i < 5; ++i)
  {
    QPointF pt(0.5 + 0.5 * cos(0.8 * i * 3.14),
                0.5 + 0.5 * sin(0.8 * i * 3.14));
    aux.lineTo(pt);
    star = aux.simplified();
  }

  return star;
}

QPainterPath recalculateMark(const QPainterPath& markShape, const int& markSize)
{
  QMatrix m;
  m.scale(markSize*2, markSize*2);

  return m.map(markShape);
}

QImage getStarImage(const QSize& size, const QColor& c)
{
  QImage img(size, QImage::Format_ARGB32_Premultiplied);

  img.fill(Qt::color0);

  QPainter p(&img);
  QPainterPath star = getStar();
  star = recalculateMark(star, size.height()/2);

  p.translate(img.rect().center()-star.boundingRect().center());

  p.fillPath(star, Qt::color1);
  p.setPen(QPen(Qt::color1, 1));
  p.drawPath(star);

  p.fillPath(star, c);
  p.setPen(QPen(Qt::black, 1));
  p.drawPath(star);

  return img;
}

StarDelegate::StarDelegate(const size_t nClass) :
te::qt::widgets::HLDelegateDecorator(te::qt::widgets::HLDelegateDecorator::getDelegate(nClass))
{
  init();
}

StarDelegate::StarDelegate(te::qt::widgets::HighlightDelegate* decorated) :
te::qt::widgets::HLDelegateDecorator(decorated)
{
  init();
}

void StarDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  if(toHighlight(index))
  {
    QItemDelegate::paint(painter, option, index);

    if(index.column() == 0)
    {
      QRect r = option.rect;

      if(r.size() != m_img.size())
        m_img = getStarImage(r.size(), m_color);

      painter->save();
      painter->setOpacity(.5);
      painter->drawImage(r, m_img);
      painter->restore();
    }
  }
  else
    HLDelegateDecorator::paint(painter, option, index);
}

void StarDelegate::setHighlightColor(const QColor & value)
{
  HighlightDelegate::setHighlightColor(value);
  m_img = getStarImage(m_img.size(), m_color);
}

void StarDelegate::init()
{
  m_img = getStarImage(QSize(10,10), m_color);
  setGroupName(tr("Starred"));
}
