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
  \file ItemObserver.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEM_OBSERVER_H 
#define __TERRALIB_LAYOUT_INTERNAL_ITEM_OBSERVER_H

// TerraLib
#include "Observer.h"
#include "../../../../geometry/Coord2D.h"

namespace te
{
  namespace layout
  {
    class Observable;
    class ItemController;
    class Properties;

    class ItemObserver : Observer
    {
      public:
        ItemObserver(ItemController* controller = 0, Observable* o = 0);
        virtual ~ItemObserver();

        virtual void updateObserver(ContextItem context) = 0;  

        virtual std::string getNameClass();

        /*
          return coordinates llx, ury
        */
        virtual te::gm::Coord2D getPosition() = 0;

        virtual std::string getName();

        virtual te::layout::Properties* getProperties() const;	
        virtual void updateProperties(te::layout::Properties* properties);
        virtual void redraw();
        
        virtual void setPrintable(bool print);
        virtual bool isPrintable();

        virtual void setCanChangeGraphicOrder(bool canChange);
        virtual bool isCanChangeGraphicOrder();

        virtual ItemController*	getController();
        virtual Observable*	getModel();
        
        virtual void refresh();

        bool isCanZoom();

        void setCanZoom(bool zoom);

      protected:

        virtual void setZValueItem(int z);

        virtual int getZValueItem() = 0;

        te::layout::ItemController*	m_controller;
        te::layout::Observable*		m_model;
        double					m_ppi;
        bool            m_printable;
        bool            m_canChangeGraphicOrder; //!< Define if item can send to back ou bring to front
        bool            m_canZoom;
        std::string     m_nameClass;
    };
  }
}

#endif