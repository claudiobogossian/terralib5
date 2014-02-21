#ifndef ITEMGROUPLAYOUTCONTROLLER_H
#define ITEMGROUPLAYOUTCONTROLLER_H

#include "LayoutItemController.h"

namespace te
{
  namespace layout
  {
    class ItemGroupLayoutController : public LayoutItemController
    {
      public:

        ItemGroupLayoutController( LayoutItemModelObservable* o );
        virtual ~ItemGroupLayoutController();

        virtual void setPosition(const double& x, const double& y);
    };
  }
}

#endif //ITEMGROUPLAYOUTCONTROLLER_H