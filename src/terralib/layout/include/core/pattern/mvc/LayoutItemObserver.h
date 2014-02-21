#ifndef LAYOUTITEM_H
#define LAYOUTITEM_H

#include "LayoutProperties.h"
#include "ContextLayoutItem.h"
#include "../../../../../geometry/Coord2D.h"

namespace te
{
  namespace layout
  {
    class LayoutItemModelObservable;
    class LayoutItemController;

    class LayoutItemObserver
    {
      public:
        LayoutItemObserver(LayoutItemController* controller = 0, LayoutItemModelObservable* o = 0);
        virtual ~LayoutItemObserver();
        virtual void updateObserver(ContextLayoutItem context) = 0;

        virtual void setItemPosition(const double& x, const double& y);

        /*
          return coordinates llx, ury
        */
        virtual te::gm::Coord2D getPosition() = 0;

        virtual te::layout::LayoutProperties toString();	
        virtual void setProperties(te::layout::LayoutProperties properties);
        virtual void redraw(const double& scaleFactor = 1.);
        virtual void refresh();

        virtual void setPPI(const double& ppi);

      protected:
        virtual void setPosition(const double& x, const double& y) = 0;

      protected:
        te::layout::LayoutItemController*	_controller;
        te::layout::LayoutItemModelObservable*		_model;
        double					_ppi;
    };
  }
}

#endif