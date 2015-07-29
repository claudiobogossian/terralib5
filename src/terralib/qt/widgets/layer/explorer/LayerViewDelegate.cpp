#include "LayerViewDelegate.h"
#include "LayerItem.h"
#include "LegendItem.h"
#include "ColorMapSliceItem.h"
#include "ChartSliceItem.h"

#include "../../../../maptools/AbstractLayer.h"

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
  m_queryLayer = QIcon::fromTheme("query-layer");
  m_tabularLayer = QIcon::fromTheme("dataset-layer-tabular");
  m_invalidLayer = QIcon::fromTheme("dataset-layer-invalid");
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
    (item->getType() == "FOLDER") ? m_folder : getIcon(index);

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

QIcon te::qt::widgets::LayerViewDelegate::getIcon(const QModelIndex & index) const
{
  if(!index.isValid())
    throw;

  TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
  
  if(item->getType() != "LAYER")
    throw;

  te::map::AbstractLayerPtr l = ((LayerItem*)item)->getLayer();

  if(!l->isValid())
    return m_invalidLayer;

  if(l->getType() == "QUERYLAYER")
    return m_queryLayer;

  std::auto_ptr<te::da::DataSetType> schema = l->getSchema();

  if(!schema->hasGeom() && !schema->hasRaster() || l->getType() == "DATASETADAPTERLAYER")
    return m_tabularLayer;

  return m_layer;
}
