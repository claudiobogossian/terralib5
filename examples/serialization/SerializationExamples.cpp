#include "SerializationExamples.h"

// TerraLib
#include <terralib/fe.h>
#include <terralib/se.h>
#include <terralib/serialization/fe/Filter.h>
#include <terralib/serialization/se/Style.h>
#include <terralib/xml/Writer.h>

// STL
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

// Shows the serialization result on console
void ShowResult(const std::string& s)
{
  std::cout << "Result:" << std::endl;
  std::cout << s << std::endl;
}

// Saves the serialization result to a file path
void SaveResult(const std::string& s, const std::string& path)
{
  if(path.empty())
    return;

  std::ofstream ofs(path);
  ofs << s;
}

void EncodeFilter(const std::string& path)
{
  /* Creating an OGC Filter Expression */
  
  // (1): nome = 'MINAS GERAIS'
  te::fe::PropertyName* state = new te::fe::PropertyName("state");
  te::fe::Literal* stateName = new te::fe::Literal("MINAS GERAIS");
  te::fe::BinaryComparisonOp* stateEqual = new te::fe::BinaryComparisonOp(te::fe::Globals::sm_propertyIsEqualTo, state, stateName);

  // (2): populacao < '2.000'
  te::fe::PropertyName* pop = new te::fe::PropertyName("population");
  te::fe::Literal* popValue = new te::fe::Literal("2.000");
  te::fe::BinaryComparisonOp* popLessThan = new te::fe::BinaryComparisonOp(te::fe::Globals::sm_propertyIsLessThan, pop, popValue);

  // (3): Joins the expression (1) and (2) using a binary logic operator AND
  te::fe::BinaryLogicOp* andOp = new te::fe::BinaryLogicOp(te::fe::Globals::sm_and, stateEqual, popLessThan);

  // (4): cidade = 'SERITINGA'
  te::fe::PropertyName* city = new te::fe::PropertyName("city");
  te::fe::Literal* cityName = new te::fe::Literal("SERITINGA");
  te::fe::BinaryComparisonOp* cityEqual = new te::fe::BinaryComparisonOp(te::fe::Globals::sm_propertyIsEqualTo, city, cityName);
  
  // (5): Joins the expression (3) and (4) using a binary logic operator OR
  te::fe::BinaryLogicOp* orOp = new te::fe::BinaryLogicOp(te::fe::Globals::sm_or, andOp, cityEqual);

  // We have a Filter!
  te::fe::Filter* filter = new te::fe::Filter;
  filter->setOp(orOp); // (state = 'MINAS GERAIS' AND populacao < '2.000') OR (city = 'SERITINGA')

  /* Let's serialize! */
   std::ostringstream oss;

  // Serializing...
  te::xml::Writer writer(oss);
  te::serialize::Save(filter, writer);

  // Shows the serialization result
  ShowResult(oss.str());
  
  // Case path not empty, saves the serialization result to a file
  SaveResult(oss.str(), path);

  delete filter;
}

void EncodeStyle(const std::string& path)
{
  /* Creating an OGC Symbology Enconding Style */

  // Creates a PolygonSymbolizer
  te::se::Stroke* strokePolygon = te::se::CreateStroke("#000000", "2", "0.5", "2 3 2 3", "square", "miter");
  te::se::Fill* fillPolygon = te::se::CreateFill("#FF8C00", "1.0");
  te::se::PolygonSymbolizer* ps = te::se::CreatePolygonSymbolizer(strokePolygon, fillPolygon);
  ps->setGeometry(new te::fe::PropertyName("area"));
  ps->setDescription(te::se::CreateDescription("A simple polygon symbolizer example", "This symbolizer was created to show the power of TerraLib serialization module."));

  // Creates a LineSymbolizer
  te::se::Stroke* strokeLine = te::se::CreateStroke("#FF0000", "1");
  te::se::LineSymbolizer* ls = te::se::CreateLineSymbolizer(strokeLine);
  ls->setGeometry(new te::fe::PropertyName("centerline"));
  ls->setDescription(te::se::CreateDescription("A simple line symbolizer example", "This symbolizer was created to show the power of TerraLib serialization module."));

  // Creates a PointSymbolizer
  te::se::Mark* mark = te::se::CreateMark("star", te::se::CreateStroke("#000000", "1"), te::se::CreateFill("#FFFF00", "1.0"));
  te::se::Graphic* graphic = te::se::CreateGraphic(mark, "8", "45", "1.0");
  te::se::PointSymbolizer* pts = te::se::CreatePointSymbolizer(graphic);
  pts->setGeometry(new te::fe::PropertyName("location"));
  pts->setDescription(te::se::CreateDescription("A simple point symbolizer example", "This symbolizer was created to show the power of TerraLib serialization module."));

  // Creates a TextSymbolizer
  te::se::TextSymbolizer* ts = te::se::CreateTextSymbolizer("location", te::se::CreateFill("#000000", "1.0"), te::se::CreateFont("Arial", "12", te::se::Font::StyleNormal, te::se::Font::WeightBold)); 
  ts->setGeometry(new te::fe::PropertyName("location"));
  ts->setDescription(te::se::CreateDescription("A simple text symbolizer example", "This symbolizer was created to show the power of TerraLib serialization module."));

  // Creates a RasterSymbolizer
  te::se::SelectedChannel* scR = new te::se::SelectedChannel;
  scR->setSourceChannelName("0");
  te::se::SelectedChannel* scG = new te::se::SelectedChannel;
  scG->setSourceChannelName("1");
  te::se::SelectedChannel* scB = new te::se::SelectedChannel;
  scB->setSourceChannelName("2");

  te::se::ChannelSelection* cs = new te::se::ChannelSelection;
  cs->setColorCompositionType(te::se::RGB_COMPOSITION);
  cs->setRedChannel(scR);
  cs->setGreenChannel(scG);
  cs->setBlueChannel(scB);

  te::se::RasterSymbolizer* rs = new te::se::RasterSymbolizer;
  rs->setOpacity(new te::se::ParameterValue("1.0"));
  rs->setGain(new te::se::ParameterValue("1.0"));
  rs->setOffset(new te::se::ParameterValue("0.0"));
  rs->setChannelSelection(cs);
  rs->setDescription(te::se::CreateDescription("A simple raster symbolizer example", "This symbolizer was created to show the power of TerraLib serialization module."));

  // Creates a Rule
  te::se::Rule* rule = new te::se::Rule;
  rule->setName(new std::string("Rule 1"));
  rule->setMinScaleDenominator(250e3);
  rule->setMaxScaleDenominator(5e6);
  rule->push_back(ps);
  rule->push_back(ls);
  rule->push_back(pts);
  rule->push_back(ts);
  rule->push_back(rs);
  rule->setDescription(te::se::CreateDescription("A simple rule example", "This rule was created to show the power of TerraLib serialization module."));

  // We have a Style!
  te::se::Style* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);
  style->setName(new std::string("Style 1"));
  style->setDescription(te::se::CreateDescription("A simple style example", "This style was created to show the power of TerraLib serialization module."));

  /* Let's serialize! */
  std::ostringstream oss;

  // Serializing...
  te::xml::Writer writer(oss);
  te::serialize::Style::getInstance().write(style, writer);

  // Shows the serialization result
  ShowResult(oss.str());
  
  // Case path not empty, saves the serialization result to a file
  SaveResult(oss.str(), path);

  delete style;
}
