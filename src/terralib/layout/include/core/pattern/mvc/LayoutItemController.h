#ifndef LAYOUTITEMCONTROLLER_H
#define LAYOUTITEMCONTROLLER_H

#include "LayoutProperties.h"

namespace te
{
  namespace layout
  {
    class LayoutItemModelObservable;
    class LayoutItemObserver;

    class LayoutItemController 
    {
      public:

        LayoutItemController(LayoutItemModelObservable* o);
        virtual ~LayoutItemController();

        virtual void setPosition(const double& x, const double& y) = 0;
        virtual void redraw(const double& zoomFactor);
        const LayoutItemModelObservable* getModel();
        const LayoutItemObserver* getView();
        virtual void setProperties(LayoutProperties properties);

      protected:
        LayoutItemModelObservable* _model;
        LayoutItemObserver* _view;
    };
  }
}

#endif