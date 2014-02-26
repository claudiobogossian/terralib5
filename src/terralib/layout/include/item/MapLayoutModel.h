#ifndef __TERRALIB_LAYOUT_INTERNAL_MAPLAYOUTMODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_MAPLAYOUTMODEL_H

#include "LayoutItemModelObservable.h"
#include "ContextLayoutItem.h"

namespace te
{
  namespace layout
  {
    class MapLayoutModel : public LayoutItemModelObservable
    {
      public:

        MapLayoutModel();
        virtual ~MapLayoutModel();

        virtual void draw( ContextLayoutItem context );

        virtual void setPosition( const double& x, const double& y );

    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_RECTANGLELAYOUTMODEL_H