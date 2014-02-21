#ifndef OBJECTINSPECTOR_WINDOWLAYOUTCONTROLLER_H
#define OBJECTINSPECTOR_WINDOWLAYOUTCONTROLLER_H

#include "LayoutOutsideController.h"

namespace te
{
  namespace layout
  {
    class ObjectInspectorWindowLayoutController : public LayoutOutsideController
    {
    public:

	    ObjectInspectorWindowLayoutController( LayoutOutsideModelObservable* o );
	    virtual ~ObjectInspectorWindowLayoutController();

	    virtual void setPosition(const double& x, const double& y);
    };
  }
}

#endif