#ifndef LAYOUTITEMMODEL_H
#define LAYOUTITEMMODEL_H

#include <set>
#include "../../../../../geometry/Envelope.h"
#include "../../../../../geometry/Coord2D.h"
#include "../../../color/RGBAColor.h"
#include "LayoutProperties.h"
#include "ContextLayoutItem.h"

namespace te
{
  namespace layout
  {
    class LayoutItemObserver;

    class LayoutItemModelObservable
    {
      public:
        LayoutItemModelObservable();
        virtual ~LayoutItemModelObservable();

        virtual bool addObserver(LayoutItemObserver* o);
        virtual bool removeObserver(LayoutItemObserver* o);
        virtual LayoutProperties toString();

        virtual te::gm::Envelope getBox();
        virtual void setBox(te::gm::Envelope box);

        virtual void setBackgroundColor(te::color::RGBAColor color);
        virtual te::color::RGBAColor getBackgroundColor();
        virtual void setBorderColor(te::color::RGBAColor color);
        virtual te::color::RGBAColor getBorderColor();

        virtual void setName(std::string name);
        virtual std::string getName();

        virtual void setPosition(const double& x, const double& y);
        virtual void draw( ContextLayoutItem context ) = 0;

        virtual bool contains(const te::gm::Coord2D &coord) const;

        virtual void setProperties(te::layout::LayoutProperties properties);

      protected:
        virtual void notifyAll(ContextLayoutItem context);

      protected:
        std::set<LayoutItemObserver*>	_observers;
        int							          _id;
        te::gm::Envelope					_box;
        te::gm::Coord2D			      _centerCoordinate;
        te::color::RGBAColor			_backgroundColor;
        te::color::RGBAColor			_borderColor;
        std::string					      _name;
    };
  }
}

#endif