#include "LayerItem.h"

#include "ChartItem.h"
#include "ColorMapItem.h"
#include "LegendItem.h"
#include "GroupingItem.h"

#include "../../se/SymbologyPreview.h"

#include "../../../../se/RasterSymbolizer.h"
#include "../../../../se/Style.h"
#include "../../../../se/Utils.h"

te::qt::widgets::LayerItem::LayerItem(te::map::AbstractLayerPtr layer) :
  TreeItem("LAYER"),
  m_layer(layer)
{
  bool raster = m_layer->getSchema()->hasRaster();

  if(m_layer->getStyle() != 0 && !raster)
  {
    std::vector<te::se::Rule*> rules = m_layer->getStyle()->getRules();

    for(std::size_t i = 0; i != rules.size(); ++i)
      addChild(new LegendItem(rules[i]));
  }

  if(m_layer->getStyle() != 0)
  {
    te::se::RasterSymbolizer* rs = te::se::GetRasterSymbolizer(m_layer->getStyle());

    if(rs && rs->getColorMap())
      addChild(new ColorMapItem(rs->getColorMap()));
  }

  if(m_layer->getGrouping())
    addChild(new GroupingItem(m_layer->getGrouping()));

  updateChart();
}

te::qt::widgets::LayerItem::~LayerItem()
{
}

std::string te::qt::widgets::LayerItem::getAsString() const
{
  return m_layer->getTitle();
}

te::qt::widgets::VISIBLE te::qt::widgets::LayerItem::isVisible() const
{
  return (m_layer->getVisibility() == te::map::VISIBLE) ? TOTALLY : NONE;
}

void te::qt::widgets::LayerItem::setVisible(const VISIBLE& visible, const bool& updateAncestors, const bool&)
{
  m_layer->setVisibility((visible == TOTALLY) ? te::map::VISIBLE : te::map::NOT_VISIBLE);

  if(updateAncestors)
    m_parent->setVisible(visible, true);
}

Qt::ItemFlags te::qt::widgets::LayerItem::flags()
{
  return TreeItem::flags() | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsUserCheckable;
}

te::map::AbstractLayerPtr te::qt::widgets::LayerItem::getLayer() const
{
  return m_layer.get();
}

void te::qt::widgets::LayerItem::updateChart()
{
  std::vector<TreeItem*> items;

  getChildren(items, "LEGEND");

  if(m_layer->getChart() != 0)
    insertChild(new ChartItem(m_layer->getChart()), items.size());
}

int te::qt::widgets::LayerItem::updateGrouping()
{
  std::vector<TreeItem*> items;
  int pos = -1;

  getChildren(items, "CHART");

  if(!items.empty())
    pos = (*items.begin())->getPosition() + 1;
  else
  {
    getChildren(items, "LEGEND");

    if(!items.empty())
      pos = (*items.begin())->getPosition() + 1;
  }

  if(m_layer->getGrouping() != 0)
  {
    if(pos >= 0)
      insertChild(new GroupingItem(m_layer->getGrouping()), pos);
    else
      addChild(new GroupingItem(m_layer->getGrouping()));
  }

  if(m_layer->getStyle() != 0)
  {
    te::se::RasterSymbolizer* rs = te::se::GetRasterSymbolizer(m_layer->getStyle());

    if(rs && rs->getColorMap())
    {
      if(pos >= 0)
        insertChild(new ColorMapItem(rs->getColorMap()), pos + 1);
      else
        addChild(new ColorMapItem(rs->getColorMap()));
    }
  }

  return pos;
}
