#ifndef LAYOUTABSTRACTITEMFACTORY_H
#define LAYOUTABSTRACTITEMFACTORY_H

#include "LayoutItemParamsCreate.h"
#include "LayoutAbstractType.h"

namespace te
{
  namespace layout
  {
    class LayoutItemObserver;

    class LayoutAbstractItemFactory 
    {
      public:
        virtual ~LayoutAbstractItemFactory(void) {}
        virtual LayoutItemObserver* make(LayoutAbstractType type, LayoutItemParamsCreate params = LayoutItemParamsCreate()) = 0;
      };
  }
}

#endif