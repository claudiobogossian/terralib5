#ifndef LAYOUTABSTRACTITEMFACTORY_H
#define LAYOUTABSTRACTITEMFACTORY_H

#include "LayoutItemParamsCreate.h"
#include "LayoutAbstractType.h"

namespace te
{
  namespace layout
  {
    class LayoutItem;

    class LayoutAbstractItemFactory 
    {
      public:
        virtual ~LayoutAbstractItemFactory(void) {}
        virtual LayoutItem* make(LayoutAbstractType type, LayoutItemParamsCreate params = LayoutItemParamsCreate()) = 0;
      };
  }
}

#endif