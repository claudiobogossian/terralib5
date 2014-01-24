#ifndef LAYOUTVIEWCONTROLLER_H
#define LAYOUTVIEWCONTROLLER_H

#include "LayoutController.h"

namespace te
{
  namespace layout
  {
    class LayoutView;

    class LayoutViewController : public LayoutController
    {
      public:
        LayoutViewController(LayoutView* view);
        virtual ~LayoutViewController();

        virtual void createNewItem(const double& centerX, const double& centerY);

      protected:
        LayoutView* _layoutView;
    };
  }
}

#endif