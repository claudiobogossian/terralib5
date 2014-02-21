#ifndef PROPERTIESWINDOWLAYOUTCONTROLLER_H
#define PROPERTIESWINDOWLAYOUTCONTROLLER_H

#include "LayoutOutsideController.h"

namespace te
{
  namespace layout
  {
    class PropertiesWindowLayoutController : public LayoutOutsideController
    {
    public:

	    PropertiesWindowLayoutController( LayoutOutsideModelObservable* o );
	    virtual ~PropertiesWindowLayoutController();

	    virtual void setPosition(const double& x, const double& y);
    };
  }
}

#endif