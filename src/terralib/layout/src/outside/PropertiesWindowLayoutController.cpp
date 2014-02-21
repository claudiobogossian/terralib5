#include "PropertiesWindowLayoutController.h"
#include "LayoutOutsideParamsCreate.h"
#include "LayoutOutsideModelObservable.h"
#include "LayoutAbstractOutsideFactory.h"
#include "LayoutContext.h"

te::layout::PropertiesWindowLayoutController::PropertiesWindowLayoutController( LayoutOutsideModelObservable* o ) :
	LayoutOutsideController(o)
{
	LayoutAbstractOutsideFactory* factory = LayoutContext::getInstance()->getOutsideFactory(); 
	LayoutOutsideParamsCreate params(this, _model);
  if(factory)
	  _view = (LayoutOutsideObserver*)factory->make(TPPropertiesWindow, params);
}

te::layout::PropertiesWindowLayoutController::~PropertiesWindowLayoutController()
{

}

void te::layout::PropertiesWindowLayoutController::setPosition( const double& x, const double& y )
{
	_model->setPosition(x, y);
}
