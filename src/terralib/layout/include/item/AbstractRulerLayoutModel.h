#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACTRULER_LAYOUTMODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACTRULER_LAYOUTMODEL_H

#include "LayoutItemModelObservable.h"

namespace te
{
  namespace layout
  {
    class AbstractRulerLayoutModel : public LayoutItemModelObservable
    {
      public:

        virtual ~AbstractRulerLayoutModel(void) {}
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_HORIZONTALRULER_LAYOUTMODEL_H