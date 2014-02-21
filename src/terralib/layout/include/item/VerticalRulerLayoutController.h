#ifndef VERTICALRULER_LAYOUTCONTROLLER_H
#define VERTICALRULER_LAYOUTCONTROLLER_H

#include "LayoutItemController.h"

namespace te
{
  namespace layout
  {
    class VerticalRulerLayoutController : public LayoutItemController
    {
      public:

        VerticalRulerLayoutController( LayoutItemModelObservable* o );
        virtual ~VerticalRulerLayoutController();

        virtual void setPosition(const double& x, const double& y);
    };
  }
}
#endif