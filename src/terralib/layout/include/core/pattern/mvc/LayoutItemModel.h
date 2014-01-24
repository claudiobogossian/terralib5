#ifndef LAYOUTITEMMODEL_H
#define LAYOUTITEMMODEL_H

#include "LayoutObservable.h"
#include <set>
#include "Coordinate.h"
#include "../../../../../geometry/Envelope.h"

namespace te
{
  namespace layout
  {
    class LayoutObserver;

    class LayoutItemModel : public LayoutObservable
    {
      public:
        LayoutItemModel();
        virtual ~LayoutItemModel();

        virtual bool addObserver(LayoutObserver* o);
        virtual bool removeObserver(LayoutObserver* o);
        virtual LayoutProperties toString();

        virtual te::gm::Envelope* getBox();
        virtual void setBox(te::gm::Envelope* box);
        virtual int getColor();
        virtual void setColor(int color);
        virtual void setName(std::string name);
        virtual std::string getName();

        virtual void setPosition(const double& x, const double& y) = 0;
        virtual void draw( ContextLayoutItem context ) = 0;

      protected:
        virtual void notifyAll(ContextLayoutItem context);

      protected:
        std::set<LayoutObserver*>	_observers;
        int							          _id;
        te::gm::Envelope*					_box;
        Coordinate					      _centerCoordinate;
        int							          _color;
        std::string					      _name;
    };
  }
}

#endif