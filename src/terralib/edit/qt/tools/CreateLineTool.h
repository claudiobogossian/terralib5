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
  \file terralib/edit/qt/tools/CreateLineTool.h

  \brief This class implements a concrete tool to create lines.
*/

#ifndef __TERRALIB_EDIT_QT_INTERNAL_CREATELINETOOL_H
#define __TERRALIB_EDIT_QT_INTERNAL_CREATELINETOOL_H

// TerraLib
#include "../../../edit/qt/core/EditionManager.h"
#include "../../../geometry/Coord2D.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../qt/widgets/tools/AbstractTool.h"
#include "../Config.h"

// STL
#include <vector>

namespace te
{
  namespace gm
  {
    class Geometry;
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
    /*!
      \class CreateLineTool

      \brief This class implements a concrete tool to create lines.
    */
    class TEEDITQTEXPORT CreateLineTool : public te::qt::widgets::AbstractTool
    {
      Q_OBJECT

      public:

        /** @name Initializer Methods
          *  Methods related to instantiation and destruction.
          */
        //@{

        /*!
          \brief It constructs a create line tool associated with the given map display.

          \param display The map display associated with the tool.
          \param parent The tool's parent.

          \note The tool will NOT take the ownership of the given pointers.
        */
        CreateLineTool(te::edit::EditionManager* editionManager, te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, const QCursor& cursor, QObject* parent = 0);

        /*! \brief Destructor. */
        ~CreateLineTool();

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

        void draw();

        te::gm::Geometry* buildLine();

        void storeNewGeometry();

      private slots:

        void onExtentChanged();

      protected:

        te::map::AbstractLayerPtr m_layer;      //!< The layer used by this tool.
        std::vector<te::gm::Coord2D> m_coords;  //!< The coord list managed by this tool.
        te::gm::Coord2D m_lastPos;              //!< The last position captured on mouse move event.
        bool m_continuousMode;                  //!< A flag that indicates if the tool is working in 'continuous mode'. i.e. the coordinates will be acquired  from each mouseMove.
        bool m_isFinished;                      //!< A flag that indicates if the operations was finished.
        te::gm::LineString* _line;                //!< The line were the coords are added. It is a copy from the current line and it is used to optimize the feedkback speed.
        te::edit::EditionManager* m_editionManager;

        void clear();
    };

  }   // end namespace edit
}     // end namespace te

#endif  // __TERRALIB_EDIT_QT_INTERNAL_CREATELINETOOL_H
