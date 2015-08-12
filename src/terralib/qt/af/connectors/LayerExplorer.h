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
  \file terralib/qt/af/connectors/LayerExplorer.h

  \brief A connector for the te::qt::widgets::LayerExplorer class to the Application Framework.
*/

#ifndef __TERRALIB_QT_AF_CONNECTORS_INTERNAL_LAYEREXPLORER_H
#define __TERRALIB_QT_AF_CONNECTORS_INTERNAL_LAYEREXPLORER_H

// Terralib
#include "../Config.h"

#include "../../../maptools/AbstractLayer.h"

// Qt
#include <QtCore/QObject>

// STL
#include <vector>
#include <list>

// Forward declarations
class QItemSelection;

namespace te
{
  namespace qt
  {
// Forward declarations
    namespace widgets 
    { 
      class LayerExplorer;
      class AbstractTreeItem;
    }

    namespace af
    {
      namespace evt
      {
      // Forward declarations
        struct Event;
      }
      /*!
        \class LayerExplorer

        \brief A connector for the te::qt::widgets::LayerExplorer class to the Application Framework.

        It is used to listen events sent by the application framework.
        It is a proxy for the te::qt::widgets::LayerExplorer.
      */
      class TEQTAFEXPORT LayerExplorer : public QObject
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.

            \param explorer te::qt::widgets::LayerExplorer to be listened.
          */
          LayerExplorer(te::qt::widgets::LayerExplorer* explorer, QObject* parent = 0);

          /*! \brief Destructor. */
          ~LayerExplorer();

          te::qt::widgets::LayerExplorer* getExplorer() const;

          void removeLayers(const std::list<te::qt::widgets::AbstractTreeItem*>& items);

        protected slots:

          /*!
            \brief Listener to the application framewrork events.

            \param evt An event sent by framework.
          */
          void onApplicationTriggered(te::qt::af::evt::Event* evt);

          /*!
            \brief It indicates whenever there are changes in the selection of layers in the layer explorer.

            \param selectedLayers  The new list of selected layers.
          */
          void onSelectedLayersChanged(const std::list<te::map::AbstractLayerPtr>& selectedLayers);

          void onLayerVisibilityChanged(te::map::AbstractLayerPtr layer);

          void onLayerOrderChanged();

          void onTreeItemDoubleClicked(te::qt::widgets::AbstractTreeItem* item);

        //signals:

          //void selectedLayersChanged(const std::list<te::map::AbstractLayerPtr>& selectedLayers);

        protected:

          te::qt::widgets::LayerExplorer* m_explorer; //!< Pointer to a component te::qt::widgets::LayerExplorer.
      };

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif // __TERRALIB_QT_AF_CONNECTORS_INTERNAL_LAYEREXPLORER_H

