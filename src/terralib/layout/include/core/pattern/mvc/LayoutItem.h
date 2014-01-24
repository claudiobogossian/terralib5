#ifndef LAYOUTITEM_H
#define LAYOUTITEM_H

#include "LayoutObserver.h"
#include "LayoutProperties.h"
#include "Coordinate.h"

namespace te
{
  namespace layout
  {
    class LayoutObservable;
    class LayoutItemController;

    class LayoutItem : public LayoutObserver
    {
      public:
        LayoutItem(LayoutItemController* controller, LayoutObservable* o);
        virtual ~LayoutItem();
        virtual void updateObserver(ContextLayoutItem context) = 0;

        virtual void setItemPosition(const double& x, const double& y);
        virtual te::layout::Coordinate getPosition() = 0;

        virtual te::layout::LayoutProperties toString();	
        virtual void redraw(const double& scaleFactor = 1.);
        virtual void refresh();

        virtual void setPPI(const double& ppi);

      protected:
        virtual void setPosition(const double& x, const double& y) = 0;

      protected:
        te::layout::LayoutItemController*	_controller;
        te::layout::LayoutObservable*		_model;
        double					_ppi;
    };
  }
}

#endif