#include "LayoutOutsideController.h"
#include "LayoutOutsideObserver.h"

te::layout::LayoutOutsideController::LayoutOutsideController( LayoutOutsideModelObservable* o )
{
	_model = o;
}

te::layout::LayoutOutsideController::~LayoutOutsideController()
{
	if(_model)
		delete _model;
}

const te::layout::LayoutOutsideModelObservable* te::layout::LayoutOutsideController::getModel()
{
	return _model;
}

const te::layout::LayoutOutsideObserver* te::layout::LayoutOutsideController::getView()
{
	return _view;
}

