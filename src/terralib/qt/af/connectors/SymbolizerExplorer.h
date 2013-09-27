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
  \file terralib/qt/af/connectors/SymbolizerExplorer.h

  \brief A connector for the te::qt::widgets::VisualDockWidget class to the Application Framework.
*/

#ifndef __TERRALIB_QT_AF_CONNECTORS_INTERNAL_SYMBOLIZEREXPLORER_H
#define __TERRALIB_QT_AF_CONNECTORS_INTERNAL_SYMBOLIZEREXPLORER_H

// Terralib
#include "../Config.h"

// Qt
#include <QtCore/QObject>

namespace te
{
  namespace map { class AbstractLayer; }

  namespace se { class Symbolizer; }

  namespace qt
  {
// Forward declarations
    namespace widgets 
    { 
      class VisualDockWidget; 
    }

    namespace af
    {
      namespace evt
      {
      // Forward declarations
        struct Event;
      }
      /*!
        \class SymbolizerExplorer

        \brief A connector for the te::qt::widgets::VisualDockWidget class to the Application Framework.

        It is used to listen events sent by the application framework.


        \ingroup afconnector
      */
      class TEQTAFEXPORT SymbolizerExplorer : public QObject
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.

            \param explorer te::qt::widgets::SymbolizerExplorer to be listened.
          */
          SymbolizerExplorer(te::qt::widgets::VisualDockWidget* explorer, QObject* parent = 0);

          /*! \brief Destructor. */
          ~SymbolizerExplorer();

          te::qt::widgets::VisualDockWidget* getExplorer() const;

        protected slots:

          /*!
            \brief Listener to the application framewrork events.

            \param evt An event sent by framework.
          */
          void onApplicationTriggered(te::qt::af::evt::Event* evt);

          /*!
            \brief Listener to the symbolizer explorer when a symbolizer has changed

          */
          void onSymbolizerChanged();

        protected:

          te::qt::widgets::VisualDockWidget* m_explorer; //!< Pointer to a component te::qt::widgets::VisualDockWidget.
      };

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif // __TERRALIB_QT_AF_CONNECTORS_INTERNAL_SYMBOLIZEREXPLORER_H

