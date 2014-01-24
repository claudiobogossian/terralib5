#include "LayoutItemModel.h"
#include "LayoutObserver.h"
#include "../../../../../geometry/Envelope.h"

te::layout::LayoutItemModel::LayoutItemModel() :
  _color(0),
  _id(0),
  _name("Unknow")
{
  _box = new te::gm::Envelope(0,0,0,0);
}

te::layout::LayoutItemModel::~LayoutItemModel()
{

}

bool te::layout::LayoutItemModel::addObserver( LayoutObserver* o )
{
  std::pair<std::set<LayoutObserver*>::iterator,bool> p = _observers.insert(o);

  if(p.second == true)
    return true;

  return false;
}

bool te::layout::LayoutItemModel::removeObserver( LayoutObserver* o )
{
  int num = _observers.erase(o);

  if(num == 1)
    return true;

  return false;
}

void te::layout::LayoutItemModel::notifyAll( ContextLayoutItem context )
{
  std::set<LayoutObserver*>::iterator it;
  for(it = _observers.begin(); it != _observers.end(); ++it)
  {
    (*it)->updateObserver(context);
  }
}

te::layout::LayoutProperties te::layout::LayoutItemModel::toString()
{
  return LayoutProperties();
}

te::gm::Envelope* te::layout::LayoutItemModel::getBox()
{
  return _box;
}

void te::layout::LayoutItemModel::setBox(te::gm::Envelope* box)
{
  _box = box;
}

int te::layout::LayoutItemModel::getColor()
{
  return _color;
}

void te::layout::LayoutItemModel::setColor( int color )
{
  _color = color;
}

void te::layout::LayoutItemModel::setName( std::string name )
{
  _name = name;
}

std::string te::layout::LayoutItemModel::getName()
{
  return _name;
}
