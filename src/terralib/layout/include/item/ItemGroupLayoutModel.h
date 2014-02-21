#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEMGROUPLAYOUTMODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_ITEMGROUPLAYOUTMODEL_H

#include "LayoutItemModelObservable.h"
#include "ContextLayoutItem.h"

namespace te
{
  namespace layout
  {
    class ItemGroupLayoutModel : public LayoutItemModelObservable
    {
      public:

        ItemGroupLayoutModel();
        virtual ~ItemGroupLayoutModel();

        virtual void draw( ContextLayoutItem context );
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_ITEMGROUPLAYOUTMODEL_H