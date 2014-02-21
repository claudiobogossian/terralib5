#include "ToolbarWindowLayoutController.h"
#include "LayoutOutsideParamsCreate.h"
#include "LayoutOutsideModelObservable.h"
#include "LayoutAbstractOutsideFactory.h"
#include "LayoutContext.h"

te::layout::ToolbarWindowLayoutController::ToolbarWindowLayoutController( LayoutOutsideModelObservable* o ) :
	LayoutOutsideController(o)
{
	LayoutAbstractOutsideFactory* factory = LayoutContext::getInstance()->getOutsideFactory(); 
	LayoutOutsideParamsCreate params(this, _model);
  if(factory)
	  _view = (LayoutOutsideObserver*)factory->make(TPToolbar, params);
}

te::layout::ToolbarWindowLayoutController::~ToolbarWindowLayoutController()
{

}

void te::layout::ToolbarWindowLayoutController::setPosition( const double& x, const double& y )
{
	_model->setPosition(x, y);
}
