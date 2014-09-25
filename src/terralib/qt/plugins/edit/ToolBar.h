/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/plugins/edit/ToolBar.h

  \brief The main toolbar of TerraLib Edit Qt plugin.
*/

#ifndef __TERRALIB_QT_PLUGINS_EDIT_INTERNAL_TOOLBAR_H
#define __TERRALIB_QT_PLUGINS_EDIT_INTERNAL_TOOLBAR_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "Config.h"

// Qt
#include <QAction>
#include <QIcon>
#include <QList>
#include <QToolBar>

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace edit
      {
        /*!
          \class ToolBar

          \brief The main toolbar of TerraLib Edit Qt plugin.
        */
        class ToolBar : public QObject
        {
          Q_OBJECT

          public:

            ToolBar();

            ~ToolBar();

            QToolBar* get() const;

          protected:

            te::map::AbstractLayerPtr getSelectedLayer();

            void initialize();

            void initializeActions();

            void createAction(QAction*& action, const QString& tooltip, const QString& icon, bool checkable, bool enabled, const char* member);

          protected slots:

            void onEditActivated(bool checked);

            void onVertexToolActivated(bool checked);

            void onCreatePolygonToolActivated(bool checked);

            void onCreateLineToolActivated(bool checked);

            void onMoveGeometryToolActivated(bool checked);

          protected:

            QToolBar* m_toolBar;

            QAction* m_editAction;
            QAction* m_vertexToolAction;
            QAction* m_createPolygonToolAction;
            QAction* m_createLineToolAction;
            QAction* m_moveGeometryToolAction;

            QList<QAction*> m_tools;
        };

      } // end namespace edit
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

#endif //__TERRALIB_QT_PLUGINS_EDIT_INTERNAL_TOOLBAR_H