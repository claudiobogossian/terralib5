#include "LegendItem.h"

// TerraLib
#include "../../canvas/Canvas.h"
#include "../../canvas/MapDisplay.h"
#include "../../../../geometry/LineString.h"
#include "../../../../geometry/Point.h"
#include "../../../../geometry/Polygon.h"
#include "../../../../maptools/CanvasConfigurer.h"
#include "../../../../se/FeatureTypeStyle.h"
#include "../../../../se/Rule.h"
#include "../../../../se/Symbolizer.h"


te::gm::Geometry* GetGeometry(te::se::FeatureTypeStyle* style)
{
  if(style->getRule(0)->getSymbolizer(0)->getType() == "PointSymbolizer")
    return new te::gm::Point(10, 10);

  if(style->getRule(0)->getSymbolizer(0)->getType() == "LineSymbolizer")
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


QIcon GetIcon(te::se::FeatureTypeStyle* style)
{
  QIcon icon;

  te::qt::widgets::Canvas canvas(20, 20);

  std::auto_ptr<te::gm::Geometry> gm(GetGeometry(style));

  // create a canvas configurer
  te::map::CanvasConfigurer cc(&canvas);

  // number of rules defined on feature type style
  std::size_t nRules = style->getRules().size();

  for(std::size_t i = 0; i < nRules; ++i) // for each <Rule>
  {
    // the current rule
    const te::se::Rule* rule = style->getRule(i);
    // get the set of symbolizers defined on current rule

    const std::vector<te::se::Symbolizer*>& symbolizers = rule->getSymbolizers();
    std::size_t nSymbolizers = symbolizers.size();

    for(std::size_t j = 0; j < nSymbolizers; ++j) // for each <Symbolizer>
    {
      // the current symbolizer
      te::se::Symbolizer* symb = symbolizers[j];

      // let's config de canvas based on the current symbolizer
      cc.config(symb);

      canvas.draw(gm.get());
    } // end for each <Symbolizer>
  }   // end for each <Rule>

  icon = QIcon(*canvas.getPixmap());

  return icon;
}

te::qt::widgets::LegendItem::LegendItem(const std::string& label, te::se::FeatureTypeStyle* style) :
TreeItem("LEGEND"),
m_label(label)
{
  m_icon = GetIcon(style);
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
