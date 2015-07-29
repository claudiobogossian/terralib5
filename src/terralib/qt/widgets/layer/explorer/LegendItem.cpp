#include "LegendItem.h"

// TerraLib
#include "../../canvas/Canvas.h"
#include "../../canvas/MapDisplay.h"
#include "../../../../geometry/LineString.h"
#include "../../../../geometry/Point.h"
#include "../../../../geometry/Polygon.h"
#include "../../../../maptools/CanvasConfigurer.h"
#include "../../../../se/Description.h"
#include "../../../../se/FeatureTypeStyle.h"
#include "../../../../se/Rule.h"
#include "../../../../se/Symbolizer.h"

// Qt
#include <QObject>


te::gm::Geometry* GetGeometry(const te::se::Symbolizer* s)
{
  if(s->getType() == "PointSymbolizer")
    return new te::gm::Point(10, 10);

  if(s->getType() == "LineSymbolizer")
  {
    te::gm::LineString* gm = new te::gm::LineString(4, te::gm::LineStringType);

    gm->setPoint(0, 0, 15);
    gm->setPoint(1, 6, 6);
    gm->setPoint(2, 12, 15);
    gm->setPoint(3, 18, 6);

    return gm;
  }

  te::gm::Polygon* gm = new te::gm::Polygon(1, te::gm::PolygonType);
  te::gm::LinearRing* ring = new te::gm::LinearRing(5, te::gm::LineStringType);

  ring->setPoint(0, 2, 18);
  ring->setPoint(1, 2, 2);
  ring->setPoint(2, 18, 2);
  ring->setPoint(3, 18, 18);
  ring->setPoint(4, 2, 18);

  gm->setRingN(0, ring);

  return gm;
}

QIcon GetIcon(const std::vector<te::se::Symbolizer*>& symbolizers)
{
  te::qt::widgets::Canvas canvas(20, 20);

  // create a canvas configurer
  te::map::CanvasConfigurer cc(&canvas);

  // number of rules defined on feature type style
  std::size_t nSymbolizers = symbolizers.size();

  for(std::size_t j = 0; j < nSymbolizers; ++j) // for each <Symbolizer>
  {
    // the current symbolizer
    te::se::Symbolizer* symb = symbolizers[j];

    // let's config de canvas based on the current symbolizer
    cc.config(symb);

    std::auto_ptr<te::gm::Geometry> gm(GetGeometry(symb));

    canvas.draw(gm.get());
  } // end for each <Symbolizer>

  return QIcon(*canvas.getPixmap());
}

QIcon GetIcon(const te::se::Rule* rule)
{
  return GetIcon(rule->getSymbolizers());
}

te::qt::widgets::LegendItem::LegendItem(const te::se::Rule* rule) :
TreeItem("LEGEND")
{
  m_icon = GetIcon(rule);

  if(rule->getDescription())
    m_label = rule->getDescription()->getTitle();

  if(rule->getName())
    m_label = *rule->getName();

  m_label = QObject::tr("Style").toStdString();
}

te::qt::widgets::LegendItem::LegendItem(const std::string& label, const std::vector<te::se::Symbolizer*>& symbolizers):
TreeItem("LEGEND"),
m_label(label)
{
  m_icon = GetIcon(symbolizers);
}

te::qt::widgets::LegendItem::~LegendItem()
{
}

std::string te::qt::widgets::LegendItem::getAsString() const
{
  return m_label;
}

QIcon te::qt::widgets::LegendItem::getIcon() const
{
  return m_icon;
}
