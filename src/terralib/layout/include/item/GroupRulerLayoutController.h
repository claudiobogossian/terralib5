#ifndef GROUPRULER_LAYOUTCONTROLLER_H
#define GROUPRULER_LAYOUTCONTROLLER_H

#include "LayoutItemController.h"

namespace te
{
  namespace layout
  {
    class GroupRulerLayoutController : public LayoutItemController
    {
      public:

        GroupRulerLayoutController( LayoutObservable* o );
        virtual ~GroupRulerLayoutController();

        virtual void setPosition(const double& x, const double& y);
    };
  }
}
#endif //GROUPRULER_LAYOUTCONTROLLER_H