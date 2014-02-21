#include "PaperLayoutController.h"
#include "LayoutItemController.h"
#include "ContextLayoutItem.h"
#include "LayoutAbstractItemFactory.h"
#include "LayoutContext.h"
#include "LayoutItemModelObservable.h"
#include "LayoutItemParamsCreate.h"
#include "LayoutItemObserver.h"

te::layout::PaperLayoutController::PaperLayoutController( LayoutItemModelObservable* o ) :
  LayoutItemController(o)
{
  LayoutAbstractItemFactory* factory = LayoutContext::getInstance()->getItemFactory(); 
  LayoutItemParamsCreate params(this, _model);
  _view = (LayoutItemObserver*)factory->make(TPPaperItem, params);
}

te::layout::PaperLayoutController::~PaperLayoutController()
{
	
}

void te::layout::PaperLayoutController::setPosition( const double& x, const double& y )
{
  _model->setPosition(x, y);
}
