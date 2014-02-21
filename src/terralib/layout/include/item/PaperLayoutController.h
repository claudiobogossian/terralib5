#ifndef PAPERLAYOUTCONTROLLER_H
#define PAPERLAYOUTCONTROLLER_H

#include "LayoutItemController.h"

namespace te
{
  namespace layout
  {
    class PaperLayoutController : public LayoutItemController
    {
      public:

        PaperLayoutController( LayoutItemModelObservable* o );
        virtual ~PaperLayoutController();

        virtual void setPosition(const double& x, const double& y);
    };
  }
}

#endif