#include "QLayoutItemFactory.h"
#include "QRectangleLayoutItem.h"
#include <QGraphicsScene>
#include "QHorizontalRulerLayoutItem.h"
#include "QVerticalRulerLayoutItem.h"
#include "LayoutItem.h"
#include "QGroupRulerLayoutItem.h"

te::layout::QLayoutItemFactory::QLayoutItemFactory()
{

}

te::layout::QLayoutItemFactory::~QLayoutItemFactory()
{

}

te::layout::LayoutItem* te::layout::QLayoutItemFactory::make( LayoutAbstractType type, LayoutItemParamsCreate params /*= LayoutItemParamsCreate()*/ )
{
  switch (type)
  {

  case TPRetangleItem:
    {
      QRectangleLayoutItem* rectangle = new QRectangleLayoutItem(params.getController(), params.getModel());
      LayoutItem* item = (LayoutItem*)rectangle;
      return item;
      break;
    }
  case TPLegendItem:
    {
      break;
    }
  case TPMapItem:
    {
      break;
    }
  case TPHorizontalRuler:
    {
      QHorizontalRulerLayoutItem* ruler = new QHorizontalRulerLayoutItem(params.getController(), params.getModel());
      LayoutItem* itemMap = (LayoutItem*)ruler;
      return itemMap;
      break;
    }
  case TPVerticalRuler:
    {
      QVerticalRulerLayoutItem* ruler = new QVerticalRulerLayoutItem(params.getController(), params.getModel());
      LayoutItem* itemMap = (LayoutItem*)ruler;
      return itemMap;
      break;
    }
  case TPGroupRuler:
    {
      QGroupRulerLayoutItem* groupruler = new QGroupRulerLayoutItem(params.getController(), params.getModel());
      LayoutItem* itemMap = (LayoutItem*)groupruler;
      return itemMap;
      break;
    }
  case TPText:
    {
      break;
    }
  }

  return 0;
}
