#ifndef __TERRALIB_LAYOUT_INTERNAL_RECTANGLELAYOUTMODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_RECTANGLELAYOUTMODEL_H

#include "LayoutItemModel.h"
#include "ContextLayoutItem.h"

namespace te
{
  namespace layout
  {
    class RectangleLayoutModel : public LayoutItemModel
    {
      public:

        RectangleLayoutModel();
        virtual ~RectangleLayoutModel();

        virtual void draw( ContextLayoutItem context );
        virtual void setPosition(const double& x, const double& y);
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_RECTANGLELAYOUTMODEL_H