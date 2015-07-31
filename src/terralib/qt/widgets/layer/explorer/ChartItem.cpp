#include "../../../../color/RGBAColor.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../../../maptools/Chart.h"
//#include "../../../../se/Categorize.h"
//#include "../../../../se/ColorMap.h"
//#include "../../../../se/Interpolate.h"
//#include "../../../../se/InterpolationPoint.h"
//#include "../../../../se/RasterSymbolizer.h"
//#include "../../../../se/Utils.h"

#include "ChartItem.h"
#include "ChartSliceItem.h"
#include "LayerItem.h"

#include <QColor>
#include <QObject>

void AddSliceItems(te::qt::widgets::ChartItem* item, const te::map::Chart* chart)
{
  int contourWidth = (int)chart->getContourWidth();
  QColor contourColor(chart->getContourColor().getRed(), chart->getContourColor().getGreen(), chart->getContourColor().getBlue());

  std::vector<std::string> properties = chart->getProperties();

  for(std::size_t i = 0; i < properties.size(); ++i)
  {
    te::color::RGBAColor c = chart->getColor(i);
    QColor color(c.getRed(), c.getGreen(), c.getBlue());
    item->addChild(new te::qt::widgets::ChartSliceItem(properties[i].c_str(), color, contourColor, contourWidth));
  }
}

te::qt::widgets::ChartItem::ChartItem(te::map::Chart* chart) :
  TreeItem("CHART"),
  m_chart(chart)
{
  switch(m_chart->getType())
  {
    case te::map::Pie:
      m_label = QObject::tr("Pie Chart").toStdString();
    break;

    case te::map::Bar:
      m_label = QObject::tr("Bar Chart").toStdString();
    break;

    default:
      m_label = QObject::tr("Chart").toStdString();
  }

  AddSliceItems(this, m_chart);
}

te::qt::widgets::ChartItem::~ChartItem()
{
  if(m_parent == 0)
    return;

  te::map::AbstractLayerPtr layer = ((LayerItem*)m_parent)->getLayer();

  te::map::Chart* chart = layer->getChart();

  if(chart == m_chart)
    layer->setChart(0);
}

std::string te::qt::widgets::ChartItem::getAsString() const
{
  return m_label;
}

te::qt::widgets::VISIBLE te::qt::widgets::ChartItem::isVisible() const
{
  return m_chart->isVisible() ? TOTALLY : NONE;
}

void te::qt::widgets::ChartItem::setVisible(const VISIBLE& visible, const bool&, const bool&)
{
  m_chart->setVisibility(visible == TOTALLY ? true : false);
}

Qt::ItemFlags te::qt::widgets::ChartItem::flags()
{
  return TreeItem::flags() | Qt::ItemIsUserCheckable;
}
