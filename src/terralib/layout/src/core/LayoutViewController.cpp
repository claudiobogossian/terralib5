#include "LayoutViewController.h"
#include "LayoutView.h"
#include "RectangleLayoutModel.h"
#include "RectangleLayoutController.h"
#include "LayoutItemObserver.h"
#include "QRectangleLayoutItem.h"
#include "QLayoutView.h"
#include <QGraphicsItem>
#include "LayoutController.h"

te::layout::LayoutViewController::LayoutViewController(LayoutView* view)
{
  _layoutView = view;
  _layoutView->setLayoutController(this);
}

te::layout::LayoutViewController::~LayoutViewController()
{

}

void te::layout::LayoutViewController::createNewItem(const double& centerX, const double& centerY)
{
  RectangleLayoutModel* model = new RectangleLayoutModel();		
  RectangleLayoutController* controller = new RectangleLayoutController(model);
  LayoutItemObserver* item = (LayoutItemObserver*)controller->getView();
  item->setItemPosition(centerX,centerY);
  item->redraw();
}

