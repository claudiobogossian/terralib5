#include "ContextLayoutItem.h"


te::layout::ContextLayoutItem::ContextLayoutItem() :
  _zoomFactor(1.),
  _pixmap(0),
  _show(false),
  _wait(false),
  _resizeCanvas(true)
{

}

te::layout::ContextLayoutItem::ContextLayoutItem( LayoutParams params, double zoomFactor ) :
  _params(params),
  _zoomFactor(zoomFactor),
  _pixmap(0),
  _show(false),
  _wait(false),
  _resizeCanvas(true)
{

}

te::layout::ContextLayoutItem::~ContextLayoutItem()
{

}

void te::layout::ContextLayoutItem::setParams( LayoutParams params )
{
  _params = params;
}

te::layout::LayoutParams te::layout::ContextLayoutItem::getParams()
{
  return _params;
}

void te::layout::ContextLayoutItem::setZoomFactor( double zoomFactor )
{
  _zoomFactor = zoomFactor;
}

double te::layout::ContextLayoutItem::getZoomFactor()
{
  return _zoomFactor;
}

void te::layout::ContextLayoutItem::setPixmap( te::color::RGBAColor** pixmap )
{
  _pixmap = pixmap;
}

te::color::RGBAColor** te::layout::ContextLayoutItem::getPixmap()
{
  return _pixmap;
}

void te::layout::ContextLayoutItem::setShow( bool show )
{
  _show = show;
}

bool te::layout::ContextLayoutItem::getShow()
{
  return _show;
}

void te::layout::ContextLayoutItem::setWait( bool wait )
{
  _wait = wait;
}

bool te::layout::ContextLayoutItem::getWait()
{
  return _wait;
}

bool te::layout::ContextLayoutItem::isResizeCanvas()
{
  return _resizeCanvas;
}

void te::layout::ContextLayoutItem::setResizeCanvas( bool clear )
{
  _resizeCanvas = clear;
}

