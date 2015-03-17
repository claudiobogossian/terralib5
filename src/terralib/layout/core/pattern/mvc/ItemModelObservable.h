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
  \file ItemModelObservable.h
   
  \brief Abstract class to represent an observable. "Model" part of MVC component. All classes representing the model of a component must inherit from this class.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEM_MODEL_H 
#define __TERRALIB_LAYOUT_INTERNAL_ITEM_MODEL_H

// TerraLib
#include "Observable.h"
#include "../../Config.h"

// STL
#include <set>

namespace te
{
  namespace layout
  {
    class Observer;
    class SharedProperties;

    /*!
      \brief Abstract class to represent an observable. "Model" part of MVC component. All classes representing the model of a component must inherit from this class.
	  
	    \ingroup layout

      \sa te::layout::Observable
	  */
    class TELAYOUTEXPORT ItemModelObservable : public Observable
    {
      public:
        
        /*!
          \brief Constructor
        */ 
        ItemModelObservable();

        /*!
          \brief Destructor
        */ 
        virtual ~ItemModelObservable();

        /*!
          \brief Reimplemented from Observable
         */
        virtual bool addObserver(Observer* o);

        /*!
          \brief Reimplemented from Observable
         */
        virtual bool removeObserver(Observer* o);

        /*!
          \brief Reimplemented from Observable
         */
        virtual Properties* getProperties() const;

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
        virtual te::gm::Envelope getBox();

        /*!
          \brief Change the bounding rectangle.

          \param bounding rectangle
         */
        virtual void setBox(te::gm::Envelope box);

        /*!
          \brief Change the background color of the MVC component.

          \param color background color
         */
        virtual void setBackgroundColor(te::color::RGBAColor color);

        /*!
          \brief Returns the background color of the MVC component.

          \return background color
         */
        virtual te::color::RGBAColor getBackgroundColor();

        /*!
          \brief Change the background color of the MVC component.

          \param color border color
         */
        virtual void setBorderColor(te::color::RGBAColor color);

        /*!
          \brief Returns the border color of the MVC component.

          \return border color
         */
        virtual te::color::RGBAColor getBorderColor();

        /*!
          \brief Change the name of the MVC component.

          \param name name of the MVC component.
         */
        virtual void setName(std::string name);

        /*!
          \brief Reimplemented from Observable
         */
        virtual std::string getName();

        /*!
          \brief Reimplemented from Observable
         */
        virtual int getId();

        /*!
          \brief Reimplemented from Observable
         */
        virtual void setId(int id);

        /*!
          \brief Reimplemented from Observable
         */
        virtual int getHashCode();

        /*!
          \brief Change coordinate llx,lly of the MVC component.

          \param x llx
          \param y lly
         */
        virtual void setPosition(const double& x, const double& y);

        /*!
          \brief Drawing method of the MVC component.

          \param context maintaining the drawing context of a MVC component
         */
        virtual void draw( ContextItem context );

        /*!
          \brief Checks if the coordinate is contained within the bounding rectangle.

          \param coord coordinated to be verified
          \return true if contains, false otherwise
         */
        virtual bool contains(const te::gm::Coord2D &coord) const;

        /*!
          \brief Reimplemented from Observable
         */
        virtual void updateProperties(te::layout::Properties* properties);

        /*!
          \brief Reimplemented from Observable
         */
        virtual int getZValue();

        /*!
          \brief Reimplemented from Observable
         */
        virtual void setZValue(int zValue);

        /*!
          \brief Returns whether the border should be drawn or not.

          \return true if should be drawn, false otherwise
         */
        virtual bool isBorder();

        /*!
          \brief Change whether the border should be drawn or not.

          \param true if should be drawn, false otherwise
         */
        virtual void setBorder(bool value);

        /*!
          \brief Resize the bounding rectangle of the MVC component.

          \param true if should be drawn, false otherwise
         */
        virtual void resize(double w, double h);

        /*!
          \brief Reimplemented from Observable
         */
        virtual void setResizable(bool resize);

        /*!
          \brief Reimplemented from Observable
         */
        virtual bool isResizable();

        /*!
          \brief Change the value of rotation of the MVC component.

          \param value of rotation
         */
        virtual void setAngle(double angle);

        /*!
          \brief Returns the value of rotation.

          \return value of rotation
         */
        virtual double getAngle();

        /*!
          \brief Returns the value of old rotation.

          \return value of old rotation
         */
        virtual double getOldAngle();

        /*!
          \brief Returns the value of old position.

          \return value of old position
         */
        virtual te::gm::Coord2D getOldPos();
                
      protected:

        /*!
          \brief Reimplemented from Observable
         */
        virtual void notifyAll(ContextItem context);

        virtual int calculateHashCode();

        /*!
          \brief Draws the background of the MVC component.

          \param context maintaining the drawing context of a MVC component
         */
        virtual void drawBackground(ContextItem context);
                
      protected:
        std::set<Observer*>	      m_observers; //!< set of observers of this object
        int							          m_id; //!< hashcode
        te::gm::Envelope					m_box; //!< bounding rectangle 
        te::gm::Coord2D			      m_centerCoordinate; //!< center coordinate of the bounding rectangle
        te::color::RGBAColor			m_backgroundColor; //!< background color
        te::color::RGBAColor			m_borderColor; //!< border color
        Properties*               m_properties; //!< properties
        EnumType*                 m_type; //!< type of the MVC component
        int                       m_zValue; //!< The Z value decides the stacking order of drawing
        SharedProperties*         m_sharedProps; //!< Names of common properties among all MVC components
        bool                      m_border; //!< true if should be drawn border, false otherwise
        std::string               m_name; //!< name of the MVC component
        bool                      m_resizable; //!< true if resizable, false otherwise
        double                    m_angle; //!< value of rotation
        int                       m_hashCode;
        double                    m_oldAngle; //!< value of old rotation
        te::gm::Coord2D           m_oldPos; //!< value of old position
    };
  }
}

#endif
