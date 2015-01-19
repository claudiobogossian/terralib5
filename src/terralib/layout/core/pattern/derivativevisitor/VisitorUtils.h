/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file VisitorUtils.h
   
  \brief Utility class for visitor.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_VISITOR_UTILS_H 
#define __TERRALIB_LAYOUT_INTERNAL_VISITOR_UTILS_H

// TerraLib
#include "../../Config.h"
#include "Visitable.h"
#include "../../../../common/Singleton.h"

// Qt
#include <QGraphicsItem>
#include <QList>

namespace te
{
  namespace layout
  {
    /*!
	    \brief Utility class for visitor.
	  
	    \ingroup layout
	  */
    class TELAYOUTEXPORT VisitorUtils : public te::common::Singleton<VisitorUtils>
    {
      friend class te::common::Singleton<VisitorUtils>;

      public:

        /*!
          \brief Destructor
        */
        virtual ~VisitorUtils();

        /*!
          \brief Sets the other map (MapModel) for visitable.

          \param graphicsItems items that are visitors
          \param visitable other map (MapModel)
          \return true if changed, false otherwise
        */
        bool changeMapVisitable(QList<QGraphicsItem*> graphicsItems, Visitable* visitable);

      protected:

        /*!
          \brief Constructor
        */ 
        VisitorUtils(); 

      private:
      
        /*!
        \brief Copy constructor not allowed.
       
        \param rhs The right-hand-side copy that would be used to copy from.
        */
        VisitorUtils(const VisitorUtils& rhs);
      
        /*!
        \brief Assignment operator not allowed.
       
        \param rhs The right-hand-side copy that would be used to copy from.
       
        \return A reference to this object.
        */
        VisitorUtils& operator=(const VisitorUtils& rhs);  
    };
  }
}

#endif
