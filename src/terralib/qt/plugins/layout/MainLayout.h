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

#ifndef __TE_QT_PLUGINS_LAYOUT_INTERNAL_MAINLAYOUT_H
#define __TE_QT_PLUGINS_LAYOUT_INTERNAL_MAINLAYOUT_H

// TerraLib
#include "../../../common/Singleton.h"
#include "../../../layout/qt/core/View.h"
#include "BuildContext.h"
#include "BuildEnums.h"

// Qt
#include <QStatusBar>
#include <QGroupBox>
#include <QMenu>
#include <QWidget>

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace layout
      {   
        class DisplayDock;
        class OutsideArea;
        class ProxyProject;

        /*!
        \class 
     
        \brief A class ...within TerraLib environment.
     
        */
        class MainLayout : public te::common::Singleton<MainLayout>
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
            virtual void init(QWidget* mainWindow = 0, QMenu* mnuLayout = 0);
              
          protected:
      
            /*! \brief Constructor. */
            MainLayout();

            void createDockLayoutDisplay(QWidget* mainWindow, te::layout::View* view);

            void createLayoutContext(int width, int height);

            void createEnums();

            void finish();

            te::layout::View*         m_view;
            DisplayDock*              m_dockLayoutDisplay;
            QGroupBox*                m_groupBox;
            QStatusBar*               m_statusBar;
            te::layout::BuildContext* m_buildContext;
            OutsideArea*              m_outsideArea;  
            te::layout::BuildEnums*   m_buildEnums;
            ProxyProject*             m_proxyProject;

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
  }
}
#endif // __TERRALIB_LAYOUT_INTERNAL_MAINLAYOUT_H
