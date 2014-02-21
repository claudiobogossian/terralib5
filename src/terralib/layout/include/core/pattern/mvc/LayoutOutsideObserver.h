#ifndef LAYOUTOUTSIDE_H
#define LAYOUTOUTSIDE_H

#include "LayoutProperties.h"
#include "ContextLayoutItem.h"
#include "../../../../../geometry/Coord2D.h"

namespace te
{
  namespace layout
  {
    class LayoutOutsideController;
    class LayoutOutsideModelObservable;

    class LayoutOutsideObserver
    {
    public:
      LayoutOutsideObserver(LayoutOutsideController* controller, LayoutOutsideModelObservable* o);
      virtual ~LayoutOutsideObserver();
      virtual void updateObserver(ContextLayoutItem context) = 0;
      virtual LayoutProperties toString();
      virtual void setPosition(const double& x, const double& y) = 0;
      virtual te::gm::Coord2D getPosition() = 0;
      virtual void refresh();

      bool operator==(const LayoutOutsideObserver &other) const;
      bool operator!=(const LayoutOutsideObserver &other) const;

    protected:
      LayoutOutsideController*	_controller;
      LayoutOutsideModelObservable*		_model;
    };
  }
}

#endif