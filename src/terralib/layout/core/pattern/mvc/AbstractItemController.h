/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACTITEMCONTROLLER_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACTITEMCONTROLLER_H

// TerraLib
#include "../../Config.h"
#include "../observer/NewObserver.h"
#include "terralib/geometry/Coord2D.h"

// STL
#include <string>

// Qt
#include <QGraphicsItem>

class QVariant;

namespace te
{
  namespace layout
  {

    class AbstractItemModel;
    class AbstractItemView;
    class Property;

    /*!
      \brief Abstract class to represent an observable. "Model" part of MVC component. 
    
      \ingroup layout
    */
    class TELAYOUTEXPORT AbstractItemController : public NewObserver
    {
      public:

        /*!
          \brief Constructor
        */ 
        AbstractItemController( AbstractItemModel* model );

        
        virtual ~AbstractItemController();

        /*!
          \brief Gets the view
        */ 
        virtual AbstractItemView* getView() const;

        /*!
          \brief Gets the model
        */ 
        virtual AbstractItemModel* getModel() const;

        /*!
          \brief Gets the given property
        */
        virtual const Property& getProperty(const std::string& propertyName) const;

        /*!
          \brief Method called by the subject to inform changes in the model
        */ 
        virtual void update(const Subject* subject);

				/*!
				\brief Checks if the coordinate is contained within the bounding rectangle.

				\param coord coordinated to be verified
				\return true if contains, false otherwise
				*/
				virtual bool contains(const te::gm::Coord2D &coord) const;

      protected:

        AbstractItemModel*    m_model; //!< The model of the view
        AbstractItemView*     m_view; //!< The view
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_ABSTRACTITEMCONTROLLER_H
