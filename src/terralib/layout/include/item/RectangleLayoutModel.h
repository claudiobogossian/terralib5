#ifndef __TERRALIB_LAYOUT_INTERNAL_RECTANGLELAYOUTMODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_RECTANGLELAYOUTMODEL_H

#include "LayoutItemModelObservable.h"
#include "ContextLayoutItem.h"

namespace te
{
  namespace layout
  {
    class RectangleLayoutModel : public LayoutItemModelObservable
    {
      public:

        RectangleLayoutModel();
        virtual ~RectangleLayoutModel();

        virtual void draw( ContextLayoutItem context );
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_RECTANGLELAYOUTMODEL_H