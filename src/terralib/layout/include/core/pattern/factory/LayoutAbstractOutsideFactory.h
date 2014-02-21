#ifndef LAYOUTABSTRACTOUTSIDEFACTORY_H
#define LAYOUTABSTRACTOUTSIDEFACTORY_H

#include "LayoutOutsideParamsCreate.h"
#include "LayoutAbstractType.h"

namespace te
{
  namespace layout
  {
    class LayoutOutsideObserver;

    class LayoutAbstractOutsideFactory 
    {
    public:

      virtual ~LayoutAbstractOutsideFactory(void) {}

      virtual LayoutOutsideObserver* make(LayoutAbstractType type, LayoutOutsideParamsCreate params = LayoutOutsideParamsCreate()) = 0;
    };
  }
}

#endif