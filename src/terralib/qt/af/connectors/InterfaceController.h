/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/af/connectors/InterfaceController.h

  \brief A connector to controll all non modal interfaces.
*/

#ifndef __TERRALIB_QT_AF_CONNECTORS_INTERNAL_INTERFACECONTROLLER_H
#define __TERRALIB_QT_AF_CONNECTORS_INTERNAL_INTERFACECONTROLLER_H

// Terralib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// Qt
#include <QtCore/QObject>

//STL
#include <set>

namespace te
{
  namespace qt
  {
    // Forward declarations
    namespace widgets 
    { 
      class InterfaceController;
    }

    namespace af
    {
      namespace evt
      {
      // Forward declarations
        struct Event;
      }

      /*!
        \class InterfaceController

        \brief A connector to controll all non modal interfaces.

        It is used to listen events sent by the application framework.
      */
      class TEQTAFEXPORT InterfaceController : public QObject
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.

          */
          InterfaceController(QObject* parent = 0);

          /*! \brief Destructor. */
          ~InterfaceController();

        public:

          void addInterface(te::qt::widgets::InterfaceController* i);

          void removeInteface(te::qt::widgets::InterfaceController* i);

        protected slots:

          /*!
            \brief Listener to the application framewrork events.

            \param evt An event sent by framework.
          */
          void onApplicationTriggered(te::qt::af::evt::Event* evt);

        protected:

          /*!
            \brief This method is used to set current layer
            
          */
          void layerSelected(te::map::AbstractLayerPtr layer);

          /*!
            \brief This method is used to add a new layer
            
          */
          void layerAdded(te::map::AbstractLayerPtr layer);

          /*!
            \brief This method is used to remove a layer
            
          */
          void layerRemoved(te::map::AbstractLayerPtr layer);

        signals:

          void triggered(te::qt::af::evt::Event* e);

        protected:

          std::set<te::qt::widgets::InterfaceController*> m_interfaces;

      };

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif // __TERRALIB_QT_AF_CONNECTORS_INTERNAL_INTERFACECONTROLLER_H

