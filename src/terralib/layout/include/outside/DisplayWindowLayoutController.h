#ifndef DISPLAYWINDOWLAYOUTCONTROLLER_H
#define DISPLAYWINDOWLAYOUTCONTROLLER_H

#include "LayoutOutsideController.h"

namespace te
{
  namespace layout
  {
    class DisplayWindowLayoutController : public LayoutOutsideController
    {
    public:

	    DisplayWindowLayoutController( LayoutOutsideModelObservable* o );
	    virtual ~DisplayWindowLayoutController();

	    virtual void setPosition(const double& x, const double& y);
    };
  }
}

#endif