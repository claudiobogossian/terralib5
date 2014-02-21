#include "LayoutItemModelObservable.h"
#include "../../../../../geometry/Envelope.h"
#include "../../../../../geometry/Coord2D.h"
#include "LayoutItemObserver.h"
#include "LayoutProperty.h"

te::layout::LayoutItemModelObservable::LayoutItemModelObservable() :
  _id(0),
  _name("Unknow")
{
  _box = te::gm::Envelope(0,0,0,0);
}

te::layout::LayoutItemModelObservable::~LayoutItemModelObservable()
{

}

bool te::layout::LayoutItemModelObservable::addObserver( LayoutItemObserver* o )
{
  std::pair<std::set<LayoutItemObserver*>::iterator,bool> p = _observers.insert(o);

  if(p.second == true)
    return true;

  return false;
}

bool te::layout::LayoutItemModelObservable::removeObserver( LayoutItemObserver* o )
{
  int num = _observers.erase(o);

  if(num == 1)
    return true;

  return false;
}

void te::layout::LayoutItemModelObservable::notifyAll( ContextLayoutItem context )
{
  std::set<LayoutItemObserver*>::iterator it;
  for(it = _observers.begin(); it != _observers.end(); ++it)
  {
    (*it)->updateObserver(context);
  }
}

te::layout::LayoutProperties te::layout::LayoutItemModelObservable::toString()
{
  LayoutProperties properties;

  LayoutProperty pro_name;
  pro_name.setName(_name);
  LayoutProperty pro_label;
  pro_label.setLabel("SHOW");

  properties.addProperty(pro_name);
  properties.addProperty(pro_label);

  return properties;
}

te::gm::Envelope te::layout::LayoutItemModelObservable::getBox()
{
  return _box;
}

void te::layout::LayoutItemModelObservable::setBox(te::gm::Envelope box)
{
  _box = box;
}

te::color::RGBAColor te::layout::LayoutItemModelObservable::getBackgroundColor()
{
  return _backgroundColor;
}

void te::layout::LayoutItemModelObservable::setBackgroundColor( te::color::RGBAColor color )
{
  _backgroundColor = color;
}

void te::layout::LayoutItemModelObservable::setBorderColor( te::color::RGBAColor color )
{
  _borderColor = color;
}

te::color::RGBAColor te::layout::LayoutItemModelObservable::getBorderColor()
{
  return _borderColor;
}

void te::layout::LayoutItemModelObservable::setName( std::string name )
{
  _name = name;
}

std::string te::layout::LayoutItemModelObservable::getName()
{
  return _name;
}

void te::layout::LayoutItemModelObservable::setPosition( const double& x, const double& y )
{
  //Initial point to draw is : x1, y2, that corresponds 0,0 of local coordinate of a item
  double x1 = x; 
  double y1 = y - _box.getHeight();

  double x2 = x + _box.getWidth();
  double y2 = y;

  _box = te::gm::Envelope(x1, y1, x2, y2);
}

bool te::layout::LayoutItemModelObservable::contains( const te::gm::Coord2D &coord ) const
{
  te::gm::Envelope env(coord.x, coord.y, coord.x, coord.y);

  if(env.isValid())
    return _box.contains(env);

  return false;
}

void te::layout::LayoutItemModelObservable::setProperties( te::layout::LayoutProperties properties )
{
  std::vector<LayoutProperty> vProps = properties.getProperties();
  LayoutProperty pro_name = vProps[0];
  _name = pro_name.getName();
}
