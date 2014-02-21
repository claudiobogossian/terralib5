#include "LayoutOutsideModelObservable.h"
#include "LayoutOutsideObserver.h"
#include "LayoutProperty.h"

te::layout::LayoutOutsideModelObservable::LayoutOutsideModelObservable() :
	_color(0)
{

}

te::layout::LayoutOutsideModelObservable::~LayoutOutsideModelObservable()
{

}

bool te::layout::LayoutOutsideModelObservable::addObserver( LayoutOutsideObserver* o )
{
	std::pair<std::set<LayoutOutsideObserver*>::iterator,bool> p = _observers.insert(o);

	if(p.second == true)
		return true;

	return false;
}

bool te::layout::LayoutOutsideModelObservable::removeObserver( LayoutOutsideObserver* o )
{
	int num = _observers.erase(o);

	if(num == 1)
		return true;

	return false;
}

void te::layout::LayoutOutsideModelObservable::notifyAll( ContextLayoutItem context )
{
	std::set<LayoutOutsideObserver*>::iterator it;
	for(it = _observers.begin(); it != _observers.end(); ++it)
	{
		(*it)->updateObserver(context);
	}
}

te::layout::LayoutProperties te::layout::LayoutOutsideModelObservable::toString()
{
  LayoutProperties properties;

  LayoutProperty pro_name;
  pro_name.setName("Oi");
  LayoutProperty pro_label;
  pro_label.setLabel("SHOW");

  properties.addProperty(pro_name);
  properties.addProperty(pro_label);

  return properties;
}

te::gm::Envelope te::layout::LayoutOutsideModelObservable::getBox()
{
	return _box;
}

void te::layout::LayoutOutsideModelObservable::setBox(te::gm::Envelope box)
{
	_box = box;
}

int te::layout::LayoutOutsideModelObservable::getColor()
{
	return _color;
}

void te::layout::LayoutOutsideModelObservable::setColor( int color )
{
	_color = color;
}

bool te::layout::LayoutOutsideModelObservable::operator==(const LayoutOutsideModelObservable &other) const {

	/*if(*this == other)
		return true;*/
	return false;
}

bool te::layout::LayoutOutsideModelObservable::operator!=(const LayoutOutsideModelObservable &other) const {

	/*if(!(*this == other))
		return true;*/
	return false;
}