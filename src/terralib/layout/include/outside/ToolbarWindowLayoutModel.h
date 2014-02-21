#ifndef TOOLBARWINDOWLAYOUTMODEL_H
#define TOOLBARWINDOWLAYOUTMODEL_H

#include "LayoutOutsideModelObservable.h"
#include "ContextLayoutItem.h"
#include "LayoutProperties.h"

namespace te
{
  namespace layout
  {
    class ToolbarWindowLayoutModel : public LayoutOutsideModelObservable
    {
    public:

      ToolbarWindowLayoutModel();
      virtual ~ToolbarWindowLayoutModel();

      virtual void updateObserver(ContextLayoutItem context);
      virtual void setPosition(const double& x, const double& y);

    protected:
      LayoutProperties _properties;
    };
  }
}

#endif