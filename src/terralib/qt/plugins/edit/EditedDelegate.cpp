#include "EditedDelegate.h"

#include "../../../common/PlatformUtils.h"
#include "../../widgets/layer/explorer/LayerItem.h"

#include <QPainter>


QPainterPath GetStar()
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

QPainterPath RecalculateMark(const QPainterPath& markShape, const int& markSize)
{
  QMatrix m;
  m.scale(markSize*2, markSize*2);

  return m.map(markShape);
}

QImage GetStarImage(const QSize& size, const QColor& c)
{
  QImage img(size, QImage::Format_ARGB32_Premultiplied);

  img.fill(Qt::color0);

  QPainter p(&img);
  QPainterPath star = GetStar();
  star = RecalculateMark(star, size.height()/2);

  p.translate(img.rect().center()-star.boundingRect().center());

  p.fillPath(star, Qt::color1);
  p.setPen(QPen(Qt::color1, 1));
  p.drawPath(star);

  p.fillPath(star, c);
  p.setPen(QPen(Qt::black, 1));
  p.drawPath(star);

  return img;
}

QIcon GetIcon(const bool& stash)
{
  QPixmap p1 = QIcon::fromTheme("dataset-layer").pixmap(20, 20);
  QPixmap p2;

  if(stash)
    p2 = QPixmap(te::common::FindInTerraLibPath("resources/images/png/stash.png").c_str()).scaled(15, 15, Qt::KeepAspectRatio);
  else
    p2 = QPixmap(te::common::FindInTerraLibPath("resources/images/png/mem_edit.png").c_str()).scaled(15, 15, Qt::KeepAspectRatio);

  QPainter pt(&p1);

  pt.drawPixmap(5, 5, p2);

  pt.end();

  return p1;
}


EditDelegate::EditDelegate(QStyledItemDelegate* decorated, QObject* parent) :
  te::common::Decorator<QStyledItemDelegate>(decorated)
{
  setParent(parent);

  m_stash = GetIcon(true);
  m_memory = GetIcon(false);
}

EditDelegate::~EditDelegate()
{

}

void EditDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  te::qt::widgets::TreeItem* item = static_cast<te::qt::widgets::TreeItem*>(index.internalPointer());
  QStyleOptionViewItem opt(option);

  if(item->getType() == "LAYER")
  {
    te::map::AbstractLayerPtr l = ((te::qt::widgets::LayerItem*)item)->getLayer();

    if(m_stashed.find(l->getTitle()) != m_stashed.end())
    {
      opt.icon = m_stash;
      opt.palette.setColor(QPalette::Text, Qt::green);
      opt.font.setItalic(true);
      opt.font.setBold(true);
    }
    else if(m_edited.find(l->getTitle()) != m_edited.end())
    {
      opt.icon = m_memory;
      opt.palette.setColor(QPalette::Text, Qt::red);
      opt.font.setItalic(true);
      opt.font.setBold(true);
    }
  }

  if(m_decorated != 0)
    m_decorated->paint(painter, opt, index);
  else
    QStyledItemDelegate::paint(painter, opt, index);
}

void EditDelegate::addStashed(const std::string& lName)
{
  m_stashed.insert(lName);
  removeEdited(lName);
}

void EditDelegate::removeStashed(const std::string& lName)
{
  m_stashed.erase(lName);
}

void EditDelegate::addEdited(const std::string& lName)
{
  m_edited.insert(lName);
  removeStashed(lName);
}

void EditDelegate::removeEdited(const std::string& lName)
{
  m_edited.erase(lName);
}

bool EditDelegate::isStached(const std::string& lName)
{
  return (m_stashed.find(lName) != m_stashed.end());
}
