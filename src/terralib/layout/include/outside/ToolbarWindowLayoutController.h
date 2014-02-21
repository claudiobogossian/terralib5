#ifndef TOOLBARWINDOWLAYOUTCONTROLLER_H
#define TOOLBARWINDOWLAYOUTCONTROLLER_H

#include "LayoutOutsideController.h"

namespace te
{
  namespace layout
  {
    class ToolbarWindowLayoutController : public LayoutOutsideController
    {
    public:

	    ToolbarWindowLayoutController( LayoutOutsideModelObservable* o );
	    virtual ~ToolbarWindowLayoutController();

	    virtual void setPosition(const double& x, const double& y);
    };
  }
}

#endif