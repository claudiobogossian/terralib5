#include "DisplayWindowLayoutController.h"
#include "LayoutOutsideParamsCreate.h"
#include "LayoutOutsideModelObservable.h"
#include "LayoutAbstractOutsideFactory.h"
#include "LayoutContext.h"

te::layout::DisplayWindowLayoutController::DisplayWindowLayoutController( LayoutOutsideModelObservable* o ) :
	LayoutOutsideController(o)
{
	LayoutAbstractOutsideFactory* factory = LayoutContext::getInstance()->getOutsideFactory(); 
	LayoutOutsideParamsCreate params(this, _model);
  if(factory)
	  _view = (LayoutOutsideObserver*)factory->make(TPDisplayWindow, params);
}

te::layout::DisplayWindowLayoutController::~DisplayWindowLayoutController()
{

}

void te::layout::DisplayWindowLayoutController::setPosition( const double& x, const double& y )
{
	_model->setPosition(x, y);
}
