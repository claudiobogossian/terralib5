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
  \file AbstractOutsideModel.h
   
  \brief Abstract class to represent an observable. "Model" part of MVC widget. All classes representing the model of a widget must inherit from this class.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_OUTSIDE_MODEL_H 
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_OUTSIDE_MODEL_H

// TerraLib
#include "../../property/Properties.h"
#include "../../ContextItem.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../geometry/Coord2D.h"
#include "../../Config.h"
#include "../observer/Subject.h"

// STL
#include <set>

namespace te
{
  namespace layout
  {
    class Observer;

    /*!
      \brief Abstract class to represent an observable. "Model" part of MVC widget. All classes representing the model of a widget must inherit from this class.
    
      \ingroup layout

      \sa te::layout::Observable
    */
    class TELAYOUTEXPORT AbstractOutsideModel : public Subject
    {
    public:

      /*!
          \brief Constructor
       */ 
      AbstractOutsideModel();

      /*!
          \brief Destructor
       */ 
      virtual ~AbstractOutsideModel();
      
      /*!
          \brief Reimplemented from Observable
       */
      virtual te::gm::Envelope getBox();
      
      /*!
          \brief Change the bounding rectangle.

          \param bounding rectangle
       */
      virtual void setBox(te::gm::Envelope box);

      /*!
          \brief Returns the background color of the MVC widget.

          \return background color
       */
      virtual int getColor();
      
      /*!
          \brief Change the background color of the MVC widget.

          \param color background color
       */
      virtual void setColor(int color);

      /*!
          \brief Change coordinate llx,lly of the MVC widget.

          \param x llx
          \param y lly
       */
      virtual void setPosition(const double& x, const double& y);
      
      /*!
          \brief Reimplemented from Observable
       */
      virtual EnumType* getType();
      
      /*!
          \brief Reimplemented from Observable
       */
      virtual void setType(EnumType* type);

      /*!
          \brief Reimplemented from Observable
       */
      virtual std::string getName();

      /*!
          \brief Reimplemented from Observable
       */
      virtual int getZValue();
      
      /*!
          \brief Reimplemented from Observable
       */
      virtual void setZValue(int zValue);

      /*!
          \brief Reimplemented from Observable
       */
      virtual void updateProperties(te::layout::Properties* properties, bool notify = true);
      
      /*!
          \brief Reimplemented from Observable
       */
      virtual Properties* getProperties() const;

      /*!
          \brief Reimplemented from Observable
       */
      virtual te::layout::Properties* getPublicProperties() const;

      /*!
          \brief Reimplemented from Observable
       */
      virtual int getId();

      /*!
          \brief Reimplemented from Observable
       */
      virtual void setId( int id );

      /*!
          \brief Reimplemented from Observable
         */
      virtual int getHashCode();

      /*!
          \brief Reimplemented from Observable
       */
      virtual void setResizable(bool resize);

      /*!
          \brief Reimplemented from Observable
       */
      virtual bool isResizable();

    protected:
      
      virtual int calculateHashCode();

    protected:

      te::gm::Envelope                m_box; //!< bounding rectangle 
      te::gm::Coord2D                 m_centerCoordinate; //!< center coordinate of the bounding rectangle
      int                              m_color; //!< background color
      Properties*                      m_properties; //!< properties
      Properties*                      m_publicProperties; //!< public properties
      EnumType*                        m_type; //!< type of the MVC widget
      int                              m_zValue; //!< The Z value decides the stacking order of drawing
      int                              m_id; //!< hashcode
      bool                            m_resizable; //!< true if resizable, false otherwise
      int                              m_hashCode;
      std::string                      m_name; //!< name of the MVC widget     
    };
  }
}

#endif

