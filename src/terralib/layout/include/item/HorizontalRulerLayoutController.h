#ifndef HORIZONTALRULER_LAYOUTCONTROLLER_H
#define HORIZONTALRULER_LAYOUTCONTROLLER_H

#include "LayoutItemController.h"

namespace te
{
  namespace layout
  {
    class HorizontalRulerLayoutController : public LayoutItemController
    {
      public:

        HorizontalRulerLayoutController( LayoutItemModelObservable* o );
        virtual ~HorizontalRulerLayoutController();

        virtual void setPosition(const double& x, const double& y);
    };
  }
}
#endif