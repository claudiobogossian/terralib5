/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*! 
  \file terralib/qt/af/connectors/DataFrame.h

  \brief A connector of the te::qt::widgets::DataFrame class to the Application Framework.
*/

#ifndef __TERRALIB_QT_AF_CONNECTORS_INTERNAL_LAYOUT_EDITOR_H
#define __TERRALIB_QT_AF_CONNECTORS_INTERNAL_LAYOUT_EDITOR_H

// Terralib
#include "../../../maptools/AbstractLayer.h"
#include "../../../qt/widgets/layout/LayoutEditor.h"
#include "../Config.h"

// Qt
#include <QtCore/QObject>
#include <QtCore/QString>

namespace te
{
// Forward declaration
  namespace qt
  {
    namespace widgets
    {
      class AbstractTool;
    }

    namespace af
    {
      namespace evt
      {
      // Forward declaration
        struct Event;
      }

      /*!
        \class DataFrame
        
        \brief A connector of the te::qt::widgets::MultiThreadMapDisplay used in te::qt::widgets::DataFrame class to the Application Framework.

        It is used to listen events sent by the application framework.
        It is a proxy for the te::qt::widgets::MultiThreadMapDisplay used in te::qt::widgets::DataFrame.

        \ingroup afconnector
      */
      class TEQTAFEXPORT LayoutEditor : public QObject
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.

            \param md te::qt::widgets::MultiThreadMapDisplay to be listened.
          */
          LayoutEditor(te::qt::widgets::LayoutEditor* le);

          /*! \brief destructor. */
          ~LayoutEditor();

          te::qt::widgets::LayoutEditor* getLayoutEditor();

          /*!
            \brief Updates the current tool being used on te::qt::widgets::MapDisplay.

            \param tool The new te::qt::widgets::AbstractTool.

            \note The class will take the ownership of the given pointer.
          */
          void setCurrentTool(te::qt::widgets::AbstractTool* tool);

        protected slots:

          /*!
            \brief Listener to the application framewrork events.

            \param e An event sent by framework.
          */
          void onApplicationTriggered(te::qt::af::evt::Event* e);

          //void onDrawLayersFinished(const QMap<QString, QString>& errors);

        protected:

          //void drawLayerSelection(te::map::AbstractLayer* layer);

        protected:

          te::qt::widgets::LayoutEditor* m_layoutEditor;  //!< Pointer to a component te::qt::widgets::LayoutEditor.
          te::qt::widgets::AbstractTool* m_tool;          //!< Pointer to the current tool being used.
      };
    }
  }
}

#endif // __TERRALIB_QT_AF_CONNECTORS_INTERNAL_LAYOUT_EDITOR_H
