#include "VerticalRulerLayoutController.h"
#include "LayoutItemController.h"
#include "ContextLayoutItem.h"
#include "LayoutItemModel.h"
#include "LayoutAbstractItemFactory.h"
#include "LayoutContext.h"

te::layout::VerticalRulerLayoutController::VerticalRulerLayoutController( LayoutObservable* o ) :
  LayoutItemController(o)
{
  LayoutAbstractItemFactory* factory = LayoutContext::getInstance()->getItemFactory(); 
  LayoutItemParamsCreate params(this, _model);
  _view = (LayoutObserver*)factory->make(TPVerticalRuler, params);
}

te::layout::VerticalRulerLayoutController::~VerticalRulerLayoutController()
{

}

void te::layout::VerticalRulerLayoutController::setPosition( const double& x, const double& y )
{
  LayoutItemModel* model = dynamic_cast<LayoutItemModel*>(_model);
  model->setPosition(x, y);
}
