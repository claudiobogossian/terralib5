#include "QLayoutItemFactory.h"
#include "QRectangleLayoutItem.h"
#include <QGraphicsScene>
#include "QHorizontalRulerLayoutItem.h"
#include "QVerticalRulerLayoutItem.h"
#include "LayoutItemObserver.h"
#include "QLayoutItemGroup.h"
#include "QPaperLayoutItem.h"
#include "QMapLayoutItem.h"

te::layout::QLayoutItemFactory::QLayoutItemFactory()
{

}

te::layout::QLayoutItemFactory::~QLayoutItemFactory()
{

}

te::layout::LayoutItemObserver* te::layout::QLayoutItemFactory::make( LayoutAbstractType type, LayoutItemParamsCreate params /*= LayoutItemParamsCreate()*/ )
{
  switch (type)
  {

  case TPRetangleItem:
    {
      QRectangleLayoutItem* rectangle = new QRectangleLayoutItem(params.getController(), params.getModel());
      LayoutItemObserver* item = (LayoutItemObserver*)rectangle;
      return item;
      break;
    }
  case TPLegendItem:
    {
      break;
    }
  case TPHorizontalRuler:
    {
      QHorizontalRulerLayoutItem* ruler = new QHorizontalRulerLayoutItem(params.getController(), params.getModel());
      LayoutItemObserver* itemMap = (LayoutItemObserver*)ruler;
      return itemMap;
      break;
    }
  case TPVerticalRuler:
    {
      QVerticalRulerLayoutItem* ruler = new QVerticalRulerLayoutItem(params.getController(), params.getModel());
      LayoutItemObserver* itemMap = (LayoutItemObserver*)ruler;
      return itemMap;
      break;
    }
  case TPText:
    {
      break;
    }
  case TPItemGroup:
    {
      QLayoutItemGroup* group = new QLayoutItemGroup(params.getController(), params.getModel());
      LayoutItemObserver* itemGroup = (LayoutItemObserver*)group;
      return itemGroup;
      break;
    }
  case TPPaperItem:
    {
      QPaperLayoutItem* paper = new QPaperLayoutItem(params.getController(), params.getModel());
      LayoutItemObserver* itemPaper = (LayoutItemObserver*)paper;
      return itemPaper;
      break;
    }
  case TPMapItem:
    {
      QMapLayoutItem* map = new QMapLayoutItem(params.getController(), params.getModel());
      LayoutItemObserver* item = (LayoutItemObserver*)map;
      return item;
      break;
    }
  }

  return 0;
}
