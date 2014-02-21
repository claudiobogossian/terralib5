#include "ObjectInspectorWindowLayoutController.h"
#include "LayoutOutsideParamsCreate.h"
#include "LayoutOutsideModelObservable.h"
#include "LayoutAbstractOutsideFactory.h"
#include "LayoutContext.h"

te::layout::ObjectInspectorWindowLayoutController::ObjectInspectorWindowLayoutController( LayoutOutsideModelObservable* o ) :
	LayoutOutsideController(o)
{
	LayoutAbstractOutsideFactory* factory = LayoutContext::getInstance()->getOutsideFactory(); 
	LayoutOutsideParamsCreate params(this, _model);
  if(factory)
	  _view = (LayoutOutsideObserver*)factory->make(TPObjectInspectorWindow, params);
}

te::layout::ObjectInspectorWindowLayoutController::~ObjectInspectorWindowLayoutController()
{

}

void te::layout::ObjectInspectorWindowLayoutController::setPosition( const double& x, const double& y )
{
	_model->setPosition(x, y);
}
