#include "LayerItem.h"

#include "ChartItem.h"
#include "ColorMapItem.h"
#include "LegendItem.h"
#include "GroupingItem.h"

#include "../../se/SymbologyPreview.h"

#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../maptools/DataSetLayer.h"
#include "../../../../maptools/DataSetAdapterLayer.h"
#include "../../../../maptools/Utils.h"

#include "../../../../se/RasterSymbolizer.h"
#include "../../../../se/Style.h"
#include "../../../../se/Utils.h"

QString GetDataSetName(te::map::AbstractLayerPtr l)
{
  // Gets the data set name
  QString toolTip = QObject::tr("DataSet") + ": " + ((l->getType() == "DATASETADAPTERLAYER") ?
                                              ((te::map::DataSetAdapterLayer*)l.get())->getDataSetName().c_str() :
                                              (l->getType() == "DATASETLAYER") ?
                                              ((te::map::DataSetLayer*)l.get())->getDataSetName().c_str() :
                                              "" + QString("\n"));
  return toolTip;
}

QString GetTooltip(te::map::AbstractLayerPtr l)
{
  // Gets the connection info
  const std::string& id = l->getDataSourceId();

  te::da::DataSourceInfoPtr info = te::da::DataSourceInfoManager::getInstance().get(id);
  const std::map<std::string, std::string>& connInfo = info->getConnInfo();

  QString toolTip = QObject::tr("Connection Info") + ":\n";

  std::size_t i = 0;
  std::map<std::string, std::string>::const_iterator it;
  for(it = connInfo.begin(); it != connInfo.end(); ++it)
  {
    toolTip += it->first.c_str();
    toolTip += ": ";
    toolTip += it->second.c_str();
    ++i;

    if(i != connInfo.size())
      toolTip += "\n";
  }

  toolTip += '\n';

  toolTip += QObject::tr("SRID: ");
  toolTip += QString::number(l->getSRID());

  return toolTip;
}


QString BuildToolIip(te::qt::widgets::LayerItem* item)
{
  te::map::AbstractLayerPtr l = item->getLayer();

  if(!l->isValid())
    return QObject::tr("Invalid Layer");

  QString toolTip = GetDataSetName(l) +"\n"+ GetTooltip(l);

  return toolTip;
}

te::qt::widgets::LayerItem::LayerItem(te::map::AbstractLayerPtr layer) :
  TreeItem("LAYER"),
  m_layer(layer)
{
  if(!m_layer->isValid())
    return;

  if(m_layer->getStyle() != 0)
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

  m_toolTip = BuildToolIip(this).toStdString();
}

te::qt::widgets::LayerItem::~LayerItem()
{
  std::vector<TreeItem*> items;
  getChildren(items, "CHART");

  if(!items.empty())
    ((*items.begin())->setParent(0));

  items.clear();
  getChildren(items, "GROUPING");

  if(!items.empty())
    ((*items.begin())->setParent(0));

  items.clear();
  getChildren(items, "COLORMAP");

  if(!items.empty())
    ((*items.begin())->setParent(0));
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
        insertChild(new ColorMapItem(rs->getColorMap()), pos);
      else
      {
        TreeItem* aux = new ColorMapItem(rs->getColorMap());
        addChild(aux);

        pos = aux->getPosition();
      }
    }
  }

  return pos;
}

void te::qt::widgets::LayerItem::updateLegend()
{
  std::vector<TreeItem*> items;

  getChildren(items, "LEGEND");

  if(!items.empty())
    ((LegendItem*)(*items.begin()))->updateSymbol(m_layer->getStyle()->getRule(0));
}
