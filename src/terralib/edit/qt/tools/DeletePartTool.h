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
  \file terralib/edit/qt/tools/DeletePartTool.h

  \brief This class implements a concrete tool to delete parts of multi features (deleting polygons of a multi feature polygons for example).
*/

#ifndef __TERRALIB_EDIT_QT_INTERNAL_DELETEPARTTOOL_H
#define __TERRALIB_EDIT_QT_INTERNAL_DELETEPARTTOOL_H

// TerraLib
#ifndef Q_MOC_RUN
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Geometry.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../Utils.h"
#endif
#include "../Config.h"
#include "GeometriesUpdateTool.h"

// Qt
#include <QPointF>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class Canvas;
      class MapDisplay;
    }
  }

  namespace edit
  {
// Forward declaration
    class Feature;

    /*!
      \class DeletePartTool

      \brief This class implements a concrete tool to delete parts of multi features (deleting polygons of a multi feature polygons for example).
    */
    class TEEDITQTEXPORT DeletePartTool : public GeometriesUpdateTool
    {
      Q_OBJECT

      public:

        /** @name Initializer Methods
          *  Methods related to instantiation and destruction.
          */
        //@{

        /*!
          \brief It constructs a move geometry tool associated with the given map display.

          \param display The map display associated with the tool.
          \param parent The tool's parent.

          \note The tool will NOT take the ownership of the given pointers.
        */
        DeletePartTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent = 0);

        /*! \brief Destructor. */
        ~DeletePartTool();

        //@}

        /** @name AbstractTool Methods
          *  Methods related with tool behavior.
          */
        //@{

        bool mousePressEvent(QMouseEvent* e);

        //@}

      private:

        te::gm::Envelope m_mbr;

        te::gm::Envelope buildEnvelope(const QPointF& pos);

        void storeFeature();

        void updateCursor();

    };
  }   // end namespace edit
}     // end namespace te

#endif  // __TERRALIB_EDIT_QT_INTERNAL_DELETEPARTTOOL_H
