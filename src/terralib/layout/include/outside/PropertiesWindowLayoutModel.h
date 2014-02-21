#ifndef PROPERTIESWINDOWLAYOUTMODEL_H
#define PROPERTIESWINDOWLAYOUTMODEL_H

#include "LayoutOutsideModelObservable.h"
#include "ContextLayoutItem.h"
#include "LayoutProperties.h"

namespace te
{
  namespace layout
  {
    class PropertiesWindowLayoutModel : public LayoutOutsideModelObservable
    {
    public:

      PropertiesWindowLayoutModel();
      virtual ~PropertiesWindowLayoutModel();

      virtual void updateObserver(ContextLayoutItem context);
      virtual void setPosition(const double& x, const double& y);

    protected:
      LayoutProperties _properties;
    };
  }
}

#endif