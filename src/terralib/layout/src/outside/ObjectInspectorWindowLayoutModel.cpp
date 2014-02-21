#include "ObjectInspectorWindowLayoutModel.h"

te::layout::ObjectInspectorWindowLayoutModel::ObjectInspectorWindowLayoutModel()
{
  _box = te::gm::Envelope(0., 0., 200., 200.);
}

te::layout::ObjectInspectorWindowLayoutModel::~ObjectInspectorWindowLayoutModel()
{

}

void te::layout::ObjectInspectorWindowLayoutModel::updateObserver( ContextLayoutItem context )
{
	ContextLayoutItem contextNotify;
	contextNotify.setShow(context.getShow());
	notifyAll(contextNotify);
}

void te::layout::ObjectInspectorWindowLayoutModel::setPosition( const double& x, const double& y )
{
  double x1 = x; 
  double y1 = y - _box.getHeight();

  double x2 = x + _box.getWidth();
  double y2 = y;

  _box = te::gm::Envelope(x1, y1, x2, y2);
}

