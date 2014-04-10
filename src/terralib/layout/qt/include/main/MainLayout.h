/*  Copyright (C) 2001-2013 National Institute For Space Research (INPE) - Brazil.
 
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
 \file MainLayout.h
 
 \brief A class 
 */

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAINLAYOUT_H
#define __TERRALIB_LAYOUT_INTERNAL_MAINLAYOUT_H

// TerraLib
#include "../../../../common/Singleton.h"
#include "../../../include/core/Config.h"
#include "../../../../qt/widgets/canvas/Canvas.h"

class QWidget;
class QGroupBox;

namespace te
{
  namespace layout
  {
    class View;
    class DisplayOutside;
    class OutsideArea;
    class ItemFactory;
    class OutsideFactory;
    class TemplateFactory;
    class Utils;
    class PaperConfig;

    /*!
    \class 
     
    \brief A class ...within TerraLib environment.
     
    */
    class TELAYOUTEXPORT MainLayout : public te::common::Singleton<MainLayout>
    {
      friend class te::common::Singleton<MainLayout>;

      public:
            
        //! Destructor.
        ~MainLayout();
      
        /*!
        \brief Inializes 
       
        This methods 
        \exception te::layout::Exception if the 
        */
        virtual void init(QWidget* mainWindow = 0);
      
      protected:
      
        /*! \brief Constructor. */
        MainLayout();
        void createDockLayoutDisplay(QWidget* mainWindow, View* view);
        void createLayoutContext(int width, int height, View* view);
        void finish();

        View* m_view;
        DisplayOutside* m_dockLayoutDisplay;
        OutsideArea*    m_outsideArea;     
        ItemFactory*    m_itemFactory;
        OutsideFactory* m_outsideFactory;
        TemplateFactory* m_templateFactory;
        Utils* m_utils;
        PaperConfig* m_paperConfig;
        te::qt::widgets::Canvas* m_canvas;
        QGroupBox* m_groupBox;
        
      private:
      
        /*!
        \brief Copy constructor not allowed.
       
        \param rhs The right-hand-side copy that would be used to copy from.
        */
        MainLayout(const MainLayout& rhs);
      
        /*!
        \brief Assignment operator not allowed.
       
        \param rhs The right-hand-side copy that would be used to copy from.
       
        \return A reference to this object.
        */
        MainLayout& operator=(const MainLayout& rhs);      
    };
  }
}
#endif // __TERRALIB_LAYOUT_INTERNAL_MAINLAYOUT_H
