#include "LayoutContext.h"
#include <stddef.h>  // defines NULL
#include "LayoutAbstractItemFactory.h"
#include "LayoutUtils.h"
#include "../../../../../maptools/Canvas.h"
#include "../../../../../geometry/Envelope.h"
#include "LayoutAbstractOutsideFactory.h"

// Global static pointer used to ensure a single instance of the class.
te::layout::LayoutContext* te::layout::LayoutContext::_instance = NULL;

/** This function is called to create an instance of the class. 
Calling the constructor publicly is not allowed. The constructor 
is private and is only called by this Instance function.
*/
te::layout::LayoutContext* te::layout::LayoutContext::getInstance()
{
  if(!_instance)
  {
    _instance = new LayoutContext;
    return _instance;
  }
  return _instance;
}

te::layout::LayoutMode te::layout::LayoutContext::getMode()
{
  return _mode;
}

te::layout::LayoutScene* te::layout::LayoutContext::getScene()
{
  return _scene;
}

void te::layout::LayoutContext::setMode( LayoutMode mode )
{
  _mode = mode;
}

void te::layout::LayoutContext::setScene( LayoutScene* scene )
{
  _scene = scene;
}

double te::layout::LayoutContext::getZoomFactor()
{
  return _zoomFactor;
}

void te::layout::LayoutContext::setZoomFactor( double zoomFactor )
{
  _zoomFactor = zoomFactor;
}

te::layout::LayoutAbstractItemFactory* te::layout::LayoutContext::getItemFactory()
{
  return _itemFactory;
}

void te::layout::LayoutContext::setItemFactory( LayoutAbstractItemFactory* factory )
{
  _itemFactory = factory;
}

te::layout::LayoutAbstractOutsideFactory* te::layout::LayoutContext::getOutsideFactory()
{
  return _outsideFactory;
}

void te::layout::LayoutContext::setOutsideFactory( LayoutAbstractOutsideFactory* factory )
{
  _outsideFactory = factory;
}

te::layout::LayoutAbstractPaperType te::layout::LayoutContext::getTypePaper()
{
  return _typePaper;
}

void te::layout::LayoutContext::setTypePaper( LayoutAbstractPaperType type )
{
  _typePaper = type;
}

te::gm::Envelope* te::layout::LayoutContext::getBoxPaper()
{
  return _paperBox;
}

void te::layout::LayoutContext::setBoxPaper( te::gm::Envelope* box )
{
  _paperBox = box;
}

te::map::Canvas* te::layout::LayoutContext::getCanvas()
{
  return _canvas;
}

void te::layout::LayoutContext::setCanvas( te::map::Canvas* canvas )
{
  _canvas = canvas;
}

te::layout::LayoutUtils* te::layout::LayoutContext::getUtils()
{
  return _utils;
}

void te::layout::LayoutContext::setUtils( LayoutUtils* utils )
{
  _utils = utils;
}

te::layout::LayoutUnitsMetrics te::layout::LayoutContext::getUnitMetric()
{
  return _unitMetric;
}

void te::layout::LayoutContext::setUnitMetric( LayoutUnitsMetrics unit )
{
  _unitMetric = unit;
}

double te::layout::LayoutContext::getDpiX()
{
  return _dpiX;
}

void te::layout::LayoutContext::setDpiX( double dpiX )
{
  _dpiX = dpiX;
}

double te::layout::LayoutContext::getDpiY()
{
  return _dpiY;
}

void te::layout::LayoutContext::setDpiY( double dpiY )
{
  _dpiY = dpiY;
}

