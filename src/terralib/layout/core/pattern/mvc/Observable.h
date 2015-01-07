/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file Observable.h
   
  \brief Abstract class to represent an observable. "Model" part of MVC component. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_OBSERVABLE_H
#define __TERRALIB_LAYOUT_INTERNAL_OBSERVABLE_H

// TerraLib
#include "../../ContextItem.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../geometry/Coord2D.h"
#include "../../../../color/RGBAColor.h"
#include "../../enum/AbstractType.h"
#include "../../Utils.h"
#include "../../enum/EnumType.h"

namespace te
{
  namespace layout
  {
    class Observer; 
    class Properties;

    /*!
      \brief Abstract class to represent an observable. "Model" part of MVC component. 
	  
	    \ingroup layout
	  */
    class Observable 
    {
      public:

        /*!
          \brief Destructor
        */ 
        virtual ~Observable(void) {}

        /*!
          \brief Adds the specified observer to the set of observers for this object.
            Reimplement this function in a Observable subclass to provide the model's addObserver implementation.

          \param o specified observer
          \return true if add, false otherwise
        */
        virtual bool addObserver(Observer* o) = 0;

        /*!
          \brief Removes an observer from the set of observers of this object.
              Reimplement this function in a Observable subclass to provide the model's removeObserver implementation.

          \param o specified observer
          \return true if remove, false otherwise
        */
        virtual bool removeObserver(Observer* o) = 0;

        /*!
          \brief Returns the model state as properties.
            Reimplement this function in a Observable subclass to provide the model's getProperties implementation.

          \return properties
        */
        virtual te::layout::Properties* getProperties() const = 0;
        
        /*!
          \brief Updated model state with properties.
            Reimplement this function in a Observable subclass to provide the model's updateProperties implementation.

          \param properties
        */
        virtual void updateProperties(te::layout::Properties* properties) = 0;

        /*!
          \brief Returns the bounding rectangle of the component in scene coordinates(millimeters).
            Starting point is llx, lly.
            Reimplement this function in a Observable subclass to provide the model's getBox implementation.
 
          \return bounding rectangle of the component
        */
        virtual te::gm::Envelope getBox() = 0;
        
        /*!
          \brief Returns the type of component
            Reimplement this function in a Observable subclass to provide the model's getType implementation.

          \return Type of component
        */
        virtual EnumType* getType() = 0;
        
        /*!
          \brief Change the type of component.
            Reimplement this function in a Observable subclass to provide the model's setType implementation.

          \param type Type of component
        */
        virtual void setType(EnumType* type) = 0;

        /*!
          \brief Return the Z value.
            Reimplement this function in a Observable subclass to provide the model's getZValue implementation.

          \return Z value
        */
        virtual int getZValue() = 0;
        
        /*!
          \brief The Z value decides the stacking order of drawing.
            Reimplement this function in a Observable subclass to provide the model's addObserver implementation.

          \param Z Value
        */
        virtual void setZValue(int zValue) = 0;

        /*!
          \brief Method that returns the name of the MVC component.
            Reimplement this function in a Observable subclass to provide the model's getName implementation.
 
          \return name
        */
        virtual std::string getName() = 0;

        /*!
          \brief Returns the hashcode of a MVC component.
            Reimplement this function in a Observable subclass to provide the model's getId implementation.

          \return hashCode
        */
        virtual int getId() = 0;
        
        /*!
          \brief Change the hashcode of a MVC component.
            Reimplement this function in a Observable subclass to provide the model's setId implementation.

          \param id hashCode
        */
        virtual void setId(int id) = 0;

        /*!
          \brief Change the component state for resizable or not
            Reimplement this function in a Observable subclass to provide the model's setResizable implementation.

          \param resize true if resizable, false otherwise
        */
        virtual void setResizable(bool resize) = 0;

        /*!
          \brief Returns whether or not the component is resizable.
            Reimplement this function in a Observable subclass to provide the model's isResizable implementation.

          \return true if resizable, false otherwise
        */
        virtual bool isResizable() = 0;

      protected:

        /*!
          \brief Notifies all set of observers that the state of model changed
            Reimplement this function in a Observable subclass to provide the model's notifyAll implementation.

          \param context maintaining the drawing context of a MVC component.
        */
        virtual void notifyAll(ContextItem context) = 0;
    };

  }
}

#endif
