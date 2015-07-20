#include "LayerViewDelegate.h"
#include "LegendItem.h"

// Qt
#include <QPainter>

te::qt::widgets::LayerViewDelegate::LayerViewDelegate(QStyledItemDelegate* decorated, QObject* parent):
te::common::Decorator<QStyledItemDelegate>(decorated)
{
  setParent(parent);

  m_layer = QIcon(QIcon::fromTheme("dataset-layer"));
  m_folder = QIcon(QIcon::fromTheme("folder"));
}

te::qt::widgets::LayerViewDelegate::~LayerViewDelegate()
{
}

void te::qt::widgets::LayerViewDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  QStyleOptionViewItem opt = option;
  
  if(!index.isValid())
    return;
  
  TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
  QIcon ic = (item->getType() == "LEGEND") ? ((LegendItem*)item)->getIcon() : (item->getType() == "FOLDER") ? m_folder : m_layer;
  QSize s(20, 20);
    
  opt.icon = ic;
  opt.decorationSize = s;
  opt.features |= QStyleOptionViewItem::HasDecoration;

  if(m_decorated != 0)
  {
    m_decorated->paint(painter, opt, index);
    return;
  }

  QStyledItemDelegate::paint(painter, opt, index);
}