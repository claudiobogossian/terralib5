#ifndef MAPLAYOUTCONTROLLER_H
#define MAPLAYOUTCONTROLLER_H

#include "LayoutItemController.h"

namespace te
{
  namespace layout
  {
    class MapLayoutController : public LayoutItemController
    {
      public:

        MapLayoutController( LayoutItemModelObservable* o );
        virtual ~MapLayoutController();

        virtual void setPosition(const double& x, const double& y);
    };
  }
}

#endif