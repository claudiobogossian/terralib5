#include "LayerViewDelegate.h"
#include "LegendItem.h"
#include "ColorMapSliceItem.h"
#include "ChartSliceItem.h"

// Qt
#include <QPainter>

te::qt::widgets::LayerViewDelegate::LayerViewDelegate(QStyledItemDelegate* decorated, QObject* parent):
te::common::Decorator<QStyledItemDelegate>(decorated)
{
  setParent(parent);

  m_layer = QIcon::fromTheme("dataset-layer");
  m_folder = QIcon::fromTheme("folder");
  m_colorMap = QIcon::fromTheme("grouping");
  m_chart = QIcon::fromTheme("chart-pie");
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

  QIcon ic = (item->getType() == "LEGEND") ? ((LegendItem*)item)->getIcon() : 
    (item->getType() == "COLORMAP" || item->getType() == "GROUPING") ? m_colorMap :
    (item->getType() == "COLORMAPSLICE") ? ((ColorMapSliceItem*)item)->getIcon() :
    (item->getType() == "CHART") ? m_chart :
    (item->getType() == "CHARTSLICE") ? ((ChartSliceItem*)item)->getIcon() :
    (item->getType() == "FOLDER") ? m_folder : m_layer;

  QSize s(16, 16);
    
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