#include "LayoutProperty.h"

te::layout::LayoutProperty::LayoutProperty() :
  _name("unknow"),
  _label("unknow")
{

}

te::layout::LayoutProperty::~LayoutProperty()
{

}

std::string te::layout::LayoutProperty::getName()
{
  return _name;
}

void te::layout::LayoutProperty::setName( std::string name )
{
  _name = name;
}

std::string te::layout::LayoutProperty::getLabel()
{
  return _label;
}

void te::layout::LayoutProperty::setLabel( std::string label )
{
  _label = label;
}
