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
  \file terralib/edit/qt/tools/VertexTool.h

  \brief This class implements a concrete tool for vertex operations (move, add, etc.).
*/

#ifndef __TERRALIB_EDIT_QT_INTERNAL_VERTEXTOOL_H
#define __TERRALIB_EDIT_QT_INTERNAL_VERTEXTOOL_H

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../../../maptools/AbstractLayer.h"
#include "GeometriesUpdateTool.h"
#include "../../../sam/rtree/Index.h"
#include "../../Utils.h"
#include "../Config.h"

// STL
#include <string>
#include <vector>

namespace te
{
// Forward declarations
  namespace gm
  {
    struct Coord2D;
    class Geometry;
    class LineString;
    class Point;
  }

  namespace qt
  {
    namespace widgets
    {
      class MapDisplay;
    }
  }

  namespace edit
  {
// Forward declaration
    class Feature;

    /*!
      \class VertexTool

      \brief This class implements a concrete tool for vertex operations (move, add, etc.).
    */
    class TEEDITQTEXPORT VertexTool : public GeometriesUpdateTool
    {
      Q_OBJECT

      public:

        /*!
          \enum StageType

          \brief Defines the operation stage to this tool.
        */
        enum StageType
        {
          FEATURE_SELECTION,    /*!< Selection of feature.  */
          VERTEX_SEARCH,        /*!< Search of vertex.       */
          VERTEX_FOUND,         /*!< Vertex found.           */
          VERTEX_MOVING,        /*!< Vertex movement.        */
          VERTEX_READY_TO_ADD   /*!< Vertex add.             */
        };

        /** @name Initializer Methods
          *  Methods related to instantiation and destruction.
          */
        //@{

        /*!
          \brief It constructs a vertex tool associated with the given map display.

          \param display The map display associated with the tool.
          \param parent The tool's parent.

          \note The tool will NOT take the ownership of the given pointers.
        */
        VertexTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent = 0);

        /*! \brief Destructor. */
        ~VertexTool();

        //@}

        /** @name AbstractTool Methods
          *  Methods related with tool behavior.
          */
        //@{

        bool mousePressEvent(QMouseEvent* e);

        bool mouseMoveEvent(QMouseEvent* e);

        bool mouseReleaseEvent(QMouseEvent* e);

        bool mouseDoubleClickEvent(QMouseEvent* e);

        //@}

      private:

        void reset();

        void pickFeature(const te::map::AbstractLayerPtr& layer, const QPointF& pos);

        void pickFeature(const te::map::AbstractLayerPtr& layer, const te::gm::Envelope& env);

        void draw(te::gm::Point* virtualVertex = 0);

        te::gm::Envelope buildEnvelope(const QPointF& pos);

        void updateRTree();

        void setStage(StageType stage);

        void updateCursor();

        void storeEditedFeature();

      private slots:

        void onExtentChanged();

      private:

          te::map::AbstractLayerPtr m_layer;
          Feature* m_feature;
          std::vector<te::gm::LineString*> m_lines;
          VertexIndex m_currentVertexIndex;
          te::sam::rtree::Index<VertexIndex, 8> m_rtree;
          StageType m_currentStage;

    };

  }   // end namespace edit
}     // end namespace te

#endif  // __TERRALIB_EDIT_QT_INTERNAL_VERTEXTOOL_H
