#include "LayoutOutsideObserver.h"
#include "LayoutOutsideModelObservable.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include "LayoutOutsideController.h"

te::layout::LayoutOutsideObserver::LayoutOutsideObserver(LayoutOutsideController* controller, LayoutOutsideModelObservable* o) :
  _controller(controller),
  _model(o)
{
	_model->addObserver(this);
}

te::layout::LayoutOutsideObserver::~LayoutOutsideObserver()
{
	if(_controller)
		delete (LayoutOutsideController*)_controller;
}

te::layout::LayoutProperties te::layout::LayoutOutsideObserver::toString()
{
	return _model->toString();
}

void te::layout::LayoutOutsideObserver::refresh()
{
	te::gm::Coord2D coord = getPosition();
	_controller->setPosition(coord.x, coord.y);
}

bool te::layout::LayoutOutsideObserver::operator==(const LayoutOutsideObserver &other) const {

	/*if(*this == other)
		return true;*/
	return false;
}

bool te::layout::LayoutOutsideObserver::operator!=(const LayoutOutsideObserver &other) const {

	/*if(!(*this == other))
		return true;*/
	return false;
}