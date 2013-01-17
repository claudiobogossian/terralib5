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
  \file connectors/LayerExplorer.h

  \brief A connector of the te::qt::widgets::LayerExplorer for the application framework.
 */

#ifndef __TERRALIB_QT_AF_INTERNAL_LAYEREXPLORER_H
#define __TERRALIB_QT_AF_INTERNAL_LAYEREXPLORER_H

//! Terralib include files
#include <terralib/qt/af/Config.h>

//! Qt include files
#include <QObject>

//! Forward declarations
class QItemSelection;

namespace te
{
  namespace qt
  {
    //! Forward declarations
    namespace widgets
    {
      class LayerExplorer;
    }

    namespace af
    {
      //! Forward declarations
      class Event;

      /*!
        \class LayerExplorer
        
        \brief A connector of the te::qt::widgets::LayerExplorer for the application framework.
        
        This is used to listen events sent by application framework and acts on a te::qt::widgets::LayerExplorer object.
        
        \ingroup afconnector
      */
      class TEQTAFEXPORT LayerExplorer : public QObject
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.
            \param explorer te::qt::widgets::LayerExplorer to be listened.
          */
          LayerExplorer(te::qt::widgets::LayerExplorer* explorer);

          /*!
            \brief destructor.
          */
          ~LayerExplorer();

        protected slots:

          /*!
            \brief Listener to the application framewrork events.
            \param evt Event sent by framework.
          */
          void onApplicationTriggered(te::qt::af::Event* evt);

          /*!
            \brief Updates the tabular viewer with the data of the new layer, if the selected itens has one.
            \param selected The selected items in tabular viewer.
            \param unselected The items that were unselected.
          */
          void layerChanged(const QItemSelection& selected, const QItemSelection& unselected);

        protected:

          te::qt::widgets::LayerExplorer* m_explorer; //!< Pointer to a component te::qt::widgets::LayerExplorer.
      };
    }
  }
}

#endif //! __TERRALIB_QT_AF_INTERNAL_LAYEREXPLORER_H
