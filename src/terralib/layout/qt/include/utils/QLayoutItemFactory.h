#ifndef QLAYOUTITEMFACTORY_H
#define QLAYOUTITEMFACTORY_H

#include "LayoutAbstractItemFactory.h"

namespace te
{
  namespace layout
  {
    class LayoutItem;

    class QLayoutItemFactory : public LayoutAbstractItemFactory
    {
      public:
        QLayoutItemFactory();
        virtual ~QLayoutItemFactory();

        virtual LayoutItemObserver* make(LayoutAbstractType type, LayoutItemParamsCreate params = LayoutItemParamsCreate());
    };
  }
}

#endif