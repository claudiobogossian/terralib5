#ifndef RECTANGLELAYOUTCONTROLLER_H
#define RECTANGLELAYOUTCONTROLLER_H

#include "LayoutItemController.h"
#include "LayoutObservable.h"

namespace te
{
  namespace layout
  {
    class RectangleLayoutController : public LayoutItemController
    {
      public:

        RectangleLayoutController( LayoutObservable* o );
        virtual ~RectangleLayoutController();

        virtual void setPosition(const double& x, const double& y);
    };
  }
}

#endif