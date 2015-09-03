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
  \file terralib/qt/plugins/layout/LayoutEditorAction.h

  \brief This file defines the Transformation class
*/

#ifndef __TE_QT_PLUGINS_LAYOUT_INTERNAL_LAYOUTEDITOR_H
#define __TE_QT_PLUGINS_LAYOUT_INTERNAL_LAYOUTEDITOR_H

// TerraLib
#include "Config.h"
#include "AbstractAction.h"
#include "../../../layout/qt/default/DisplayDock.h"
#include "../../../layout/qt/default/MainLayout.h"
#include <QGroupBox>
#include <QVBoxLayout>

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace layout
      {

        /*!
          \class LayoutEditorAction

          \brief This class register the contrast action into VP Plugin.

        */
        class LayoutEditorAction : public te::qt::plugins::layout::AbstractAction
        {
          Q_OBJECT
            
          public:

            LayoutEditorAction(QMenu* menu);

            virtual ~LayoutEditorAction();

            virtual void createMenu();

          protected slots:

            virtual void onActionActivated(bool checked);

            virtual void onExit();




          protected:

          te::layout::DisplayDock*              m_dockLayoutDisplay;
          te::layout::MainLayout*               m_mainLayout;
          QGroupBox*                            m_groupBox;
          QStatusBar*                           m_statusBar;
          QVBoxLayout*                          m_verticalLayout;


        };

      } // end namespace layout
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

#endif //__TE_QT_PLUGINS_LAYOUT_INTERNAL_LAYOUTEDITOR_H
