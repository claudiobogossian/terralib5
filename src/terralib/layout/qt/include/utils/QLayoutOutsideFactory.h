#ifndef QLAYOUTOUTSIDEFACTORY_H
#define QLAYOUTOUTSIDEFACTORY_H

#include "LayoutAbstractOutsideFactory.h"

namespace te
{
  namespace layout
  {
    class QLayoutOutsideFactory : public LayoutAbstractOutsideFactory
    {
    public:
	    QLayoutOutsideFactory();
	    virtual ~QLayoutOutsideFactory();

	    virtual LayoutOutsideObserver* make(LayoutAbstractType type, LayoutOutsideParamsCreate params = LayoutOutsideParamsCreate());
    };
  }
}

#endif