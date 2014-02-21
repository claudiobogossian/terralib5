#ifndef DISPLAYWINDOWLAYOUTMODEL_H
#define DISPLAYWINDOWLAYOUTMODEL_H

#include "LayoutOutsideModelObservable.h"
#include "ContextLayoutItem.h"
#include "LayoutProperties.h"

namespace te
{
  namespace layout
  {
    class DisplayWindowLayoutModel : public LayoutOutsideModelObservable
    {
    public:

      DisplayWindowLayoutModel();
      virtual ~DisplayWindowLayoutModel();

      virtual void updateObserver(ContextLayoutItem context);
      virtual void setPosition(const double& x, const double& y);

    protected:
      LayoutProperties _properties;
    };
  }
}

#endif