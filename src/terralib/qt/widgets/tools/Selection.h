/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/tools/Selection.h

  \brief This class implements a concrete tool to select layer features using an extent.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_SELECTION_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_SELECTION_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"
#include "RubberBand.h"

// Qt
#include <QtGui/QColor>

// STL
#include <list>
#include <vector>

namespace te
{
// Forward declaration
  namespace gm
  {
    class Geometry;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declaration
    class MapDisplay;

      /*!
        \class Selection

        \brief This class implements a concrete tool to select layer features using an extent.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT Selection : public RubberBand
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a selection tool associated with the given map display.

            \param display The map display associated with the tool.
            \param cursor The tool cursor.
            \param layer The layer list that will be selected.
            \param parent The tool's parent.

            \note The tool will NOT take the ownership of the given pointers.
          */
          Selection(MapDisplay* display, const QCursor& cursor, const std::list<te::map::AbstractLayerPtr>& layers, QObject* parent = 0);

          /*! \brief Destructor. */
          ~Selection();

          //@}

          /** @name AbstractTool Methods
           *  Methods related with tool behavior.
           */
          //@{

          bool mousePressEvent(QMouseEvent* e);

          bool mouseMoveEvent(QMouseEvent* e);

          bool mouseReleaseEvent(QMouseEvent* e);

          //@}

        public slots:

          void setLayers(const std::list<te::map::AbstractLayerPtr>& layers);

        private:

          void executeSelection(const te::map::AbstractLayerPtr& layer, const te::gm::Envelope& e);

        signals:

          /*! This signal is emitted when the layer selection changed. */
          void layerSelectedObjectsChanged(const te::map::AbstractLayerPtr& layer);

        private:

          std::list<te::map::AbstractLayerPtr> m_layers;        //!< The layer list that will be selected.
          bool m_selectionStarted;                              //!< Flag that indicates if selection operation was started.
          bool m_keepPreviousSelection;                         //!< Flag that indicates if the tool must keep the previous selection.
          bool m_selectionByPointing;                           //!< Flag that indicates if the selection is by pointing.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_SELECTION_H
