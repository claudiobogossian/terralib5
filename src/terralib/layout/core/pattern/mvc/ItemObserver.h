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
  \file ItemObserver.h
   
  \brief Abstract class to represent an observer. "View" part of MVC component. All classes representing the graphics component must inherit from this class.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEM_OBSERVER_H 
#define __TERRALIB_LAYOUT_INTERNAL_ITEM_OBSERVER_H

// TerraLib
#include "Observer.h"
#include "../../../../geometry/Coord2D.h"
#include "../../../../color/RGBAColor.h"
#include "../../../qt/core/ContextObject.h"
#include "../../Config.h"

namespace te
{
  namespace layout
  {
    class Observable;
    class ItemController;
    class Properties;

    /*!
      \brief Abstract class to represent an observer. "View" part of MVC component. All classes representing the graphics component must inherit from this class.
	  
	    \ingroup layout

	    \sa te::layout::Observer
	  */
    class TELAYOUTEXPORT ItemObserver : Observer
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        ItemObserver(ItemController* controller = 0, Observable* o = 0);

        /*!
          \brief Destructor
        */ 
        virtual ~ItemObserver();

        /*!
          \brief Reimplemented from Observer
         */
        virtual void updateObserver(ContextItem context) = 0;  

        /*!
          \brief Method that returns the class name.

          \return class name
        */ 
        virtual std::string getNameClass();

        /*!
          \brief Method that returns the position llx, lly
            Reimplement this function in a ItemObserver subclass to provide the item's getPosition implementation.

          \return coordinate
        */ 
        virtual te::gm::Coord2D getPosition() = 0;
                
        /*!
          \brief Redraws the graphic component.
        */
        virtual void redraw(bool bRefresh = true);
        
        /*!
          \brief Modifies the state of the graphic component to be or not printable.

          \param print true if printable, false otherwise
        */
        virtual void setPrintable(bool print);

        /*!
          \brief Returns whether the graphic component is printable.

          \return true if printable, false otherwise
        */
        virtual bool isPrintable();

        /*!
          \brief Change the state of the graphic component to be or not send to back or bring to front.

          \param canChange true if send to back or bring to front, false otherwise
        */
        virtual void setCanChangeGraphicOrder(bool canChange);

        /*!
          \brief Returns whether the graphic component to be or not send to back or bring to front.

          \return true if send to back or bring to front, false otherwise
        */
        virtual bool isCanChangeGraphicOrder();

        /*!
          \brief Returns the "Controller" part of the MVC.

          \return controller
        */
        virtual ItemController*	getController();

        /*!
          \brief Returns the "Model" part of the MVC.

          \return model
        */
        virtual Observable*	getModel();
        
        /*!
          \brief Updated model state.

          \param true if refresh the current position, false otherwise
        */
        virtual void refresh(bool pos = true);
        
        /*!
          \brief Returns whether the graph component has the inverted matrix, otherwise the matrix scene.

          \return true if inverted, false otherwise
        */
        virtual bool isInvertedMatrix();

        /*!
          \brief Returns a image of the graphic component.
            Reimplement this function in a ItemObserver subclass to provide the item's getImage implementation.

          \param image width
          \param image height

          \return image
        */
        virtual te::color::RGBAColor** getRGBAColorImage(int &w, int &h) = 0;

        /*!
          \brief The default implementation does nothing.
         */
        virtual void recalculateBoundingRect();

        /*!
          \brief Checks if the object, passed via parameter, can be child.
         */
        virtual bool canBeChild(ItemObserver* item);

        /*!
          \brief This function is called every time the context is updated. It is usually used by the item to configure its drawings due to changes in the zoom factor or in the DPI of the device.
        */
        virtual void contextUpdated(ContextObject context);

      protected:

        /*!
          \brief The Z value decides the stacking order of drawing.

          \param drawing order
        */
        virtual void setZValueItem(int z);

        /*!
          \brief Return the Z value.
            Reimplement this function in a ItemObserver subclass to provide the item's getZValueItem implementation.

          \return Z value
        */
        virtual int getZValueItem() = 0;

        /*!
          \brief Rotates the graphic component.
            Reimplement this function in a ItemObserver subclass to provide the item's applyImage implementation.
        */
        virtual void applyRotation() = 0;

        te::layout::ItemController*	m_controller; //!< "Controller" part of MVC component.
        te::layout::Observable*		m_model; //!< "Model" part of MVC component.
        bool            m_printable; //!< State of the graphic component to be or not printable.
        bool            m_canChangeGraphicOrder; //!< Define if item can send to back or bring to front
        std::string     m_nameClass; //!< Class name
        bool            m_invertedMatrix; //!< true if inverted, false otherwise the matrix scene
    };
  }
}

#endif
