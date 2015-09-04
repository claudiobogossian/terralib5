#include "ThemeViewDelegate.h"

#include "LayerItem.h"

// Terralib 
#include "../../../../maptools/AbstractLayer.h"

// Qt
#include <QPainter>

te::qt::widgets::ThemeViewDelegate::ThemeViewDelegate(QStyledItemDelegate* decorated, QObject* parent) :
te::common::Decorator<QStyledItemDelegate>(decorated)
{
  setParent(parent);

  m_view = QIcon(QIcon::fromTheme("tl4-view"));
  m_theme = QIcon(QIcon::fromTheme("tl4-theme"));
}

te::qt::widgets::ThemeViewDelegate::~ThemeViewDelegate()
{
}

void te::qt::widgets::ThemeViewDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

  if(item->getType() == "LAYER" || item->getType() == "FOLDER")
  {
    QStyleOptionViewItem opt = option;
    opt.decorationSize = QSize(20, 20);
    opt.features |= QStyleOptionViewItem::HasDecoration;

    if(item->getType() == "LAYER")
      opt.icon = m_theme;
    else
      opt.icon = m_view;

    QStyledItemDelegate::paint(painter, opt, index);

    return;
  }

  if(m_decorated != 0)
    m_decorated->paint(painter, option, index);
}
