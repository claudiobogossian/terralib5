#include "../../../../color/ColorBar.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../../../se/Categorize.h"
#include "../../../../se/ColorMap.h"
#include "../../../../se/Interpolate.h"
#include "../../../../se/InterpolationPoint.h"
#include "../../../../se/RasterSymbolizer.h"
#include "../../../../se/Utils.h"

#include "ColorMapSliceItem.h"
#include "ColorMapItem.h"
#include "LayerItem.h"

#include <QObject>
#include <QString>

void AddSliceItems(te::qt::widgets::ColorMapItem* item, const te::se::ColorMap* cMap)
{
  if(cMap->getCategorize())
  {
    std::vector<te::se::ParameterValue*> t = cMap->getCategorize()->getThresholds();
    std::vector<te::se::ParameterValue*> tV = cMap->getCategorize()->getThresholdValues();

    for(std::size_t i = 1; i < tV.size() - 1; ++i)
    {
      double lowerLimit, upperLimit;

      if(i == 0)
      {
        lowerLimit = std::numeric_limits<double>::min();
        upperLimit = te::se::GetDouble(t[i]);
      }
      else if(i == tV.size() - 1)
      {
        lowerLimit = te::se::GetDouble(t[i - 1]);
        upperLimit = std::numeric_limits<double>::max();
      }
      else
      {
        lowerLimit = te::se::GetDouble(t[i - 1]);
        upperLimit = te::se::GetDouble(t[i]);
      }

      QString title = QString::number(lowerLimit);
      title.append(" - ");
      title.append(QString::number(upperLimit));

      std::string colorName = te::se::GetString(tV[i]);
      te::color::RGBAColor color(colorName);

      QColor qC(color.getRed(), color.getGreen(), color.getBlue());

      item->addChild(new te::qt::widgets::ColorMapSliceItem(title.toStdString(), qC));
    }
  }
  else if(cMap->getInterpolate())
  {
    std::vector<te::se::InterpolationPoint*> ip = cMap->getInterpolate()->getInterpolationPoints();

    for(std::size_t i = 0; i < ip.size() - 1; ++i)
    {
      te::se::InterpolationPoint* ipItemInit = ip[i];

      std::string colorName = te::se::GetString(ipItemInit->getValue());
      te::color::RGBAColor colorInit(colorName);

      QColor qBeg(colorInit.getRed(), colorInit.getGreen(), colorInit.getBlue());
      double lowerLimit = ipItemInit->getData();

      te::se::InterpolationPoint* ipItemEnd = ip[i + 1];

      colorName = te::se::GetString(ipItemEnd->getValue());
      te::color::RGBAColor colorEnd(colorName);
      
      QColor qEnd(colorEnd.getRed(), colorEnd.getGreen(), colorEnd.getBlue());
      double upperLimit = ipItemEnd->getData();

      QString title = QString::number(lowerLimit);
      title.append(" - ");
      title.append(QString::number(upperLimit));

      item->addChild(new te::qt::widgets::ColorMapSliceItem(title.toStdString(), qBeg, qEnd));
    }
  }
}

te::qt::widgets::ColorMapItem::ColorMapItem(const te::se::ColorMap* map) :
  TreeItem("COLORMAP"),
  m_colorMap(map)
{
  QString type;

  if(map->getCategorize())
    type = QObject::tr("Categorization");
  else if(map->getInterpolate())
    type = QObject::tr("Interpolation");

  m_label = (QObject::tr("Classification by") + " " + type).toStdString();

  AddSliceItems(this, m_colorMap);
}

te::qt::widgets::ColorMapItem::~ColorMapItem()
{
  if(m_parent == 0)
    return;

  te::map::AbstractLayerPtr layer = ((LayerItem*)m_parent)->getLayer();
  te::se::RasterSymbolizer* rs = te::se::GetRasterSymbolizer(layer->getStyle());

  te::se::ColorMap* map = rs->getColorMap();

  if(map == m_colorMap)
  {
    // If the item is a color map item, remove the all style from the layer associated to the parent of this color map item.
    layer->setStyle(0);

    rs->setColorMap(0);
  }
}

std::string te::qt::widgets::ColorMapItem::getAsString() const
{
  return m_label;
}

Qt::ItemFlags te::qt::widgets::ColorMapItem::flags()
{
  return Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
}
