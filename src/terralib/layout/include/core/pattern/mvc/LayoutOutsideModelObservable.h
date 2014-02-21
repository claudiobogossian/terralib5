#ifndef LAYOUTOUTSIDEMODEL_H
#define LAYOUTOUTSIDEMODEL_H

#include <set>
#include "../../../../../geometry/Envelope.h"
#include "../../../../../geometry/Coord2D.h"
#include "LayoutProperties.h"
#include "ContextLayoutItem.h"

namespace te
{
  namespace layout
  {
    class LayoutOutsideObserver;
    class LayoutItemObserver;

    class LayoutOutsideModelObservable 
    {
    public:

      LayoutOutsideModelObservable();
      virtual ~LayoutOutsideModelObservable();

      virtual bool addObserver(LayoutOutsideObserver* o);
      virtual bool removeObserver(LayoutOutsideObserver* o);
      virtual LayoutProperties toString();
      virtual te::gm::Envelope getBox();
      virtual void setBox(te::gm::Envelope box);
      virtual int getColor();
      virtual void setColor(int color);

      virtual void setPosition(const double& x, const double& y) = 0;

    protected:

      virtual void notifyAll(ContextLayoutItem context);

      bool operator==(const LayoutOutsideModelObservable &other) const;
      bool operator!=(const LayoutOutsideModelObservable &other) const;

    protected:
      std::set<LayoutOutsideObserver*>	_observers;
      te::gm::Envelope					_box;
      te::gm::Coord2D 					_centerCoordinate;
      int							          _color;
    };
  }
}

#endif