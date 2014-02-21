#ifndef OBJECTINSPECTOR_WINDOWLAYOUTMODEL_H
#define OBJECTINSPECTOR_WINDOWLAYOUTMODEL_H

#include "LayoutOutsideModelObservable.h"
#include "ContextLayoutItem.h"
#include "LayoutProperties.h"

namespace te
{
  namespace layout
  {
    class ObjectInspectorWindowLayoutModel : public LayoutOutsideModelObservable
    {
    public:

      ObjectInspectorWindowLayoutModel();
      virtual ~ObjectInspectorWindowLayoutModel();

      virtual void updateObserver(ContextLayoutItem context);
      virtual void setPosition(const double& x, const double& y);

    protected:
      LayoutProperties _properties;
    };
  }
}

#endif