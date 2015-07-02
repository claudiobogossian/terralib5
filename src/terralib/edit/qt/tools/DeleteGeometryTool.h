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
  \file terralib/edit/qt/tools/MoveGeometryTool.h

  \brief This class implements a concrete tool to move geometries.
*/

#ifndef __TERRALIB_EDIT_QT_INTERNAL_DELETEGEOMETRYTOOL_H
#define __TERRALIB_EDIT_QT_INTERNAL_DELETEGEOMETRYTOOL_H

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../qt/widgets/tools/AbstractTool.h"
#include "../Config.h"

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
      \class DeleteGeometryTool

      \brief This class implements a concrete tool to move geometries.
    */
    class TEEDITQTEXPORT DeleteGeometryTool : public te::qt::widgets::AbstractTool
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
        DeleteGeometryTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent = 0);

        /*! \brief Destructor. */
        ~DeleteGeometryTool();

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

        void pickFeature(const te::map::AbstractLayerPtr& layer);

        te::gm::Envelope buildEnvelope(const QPointF& pos);

        void draw();

        void storeRemovedFeature();

      private slots:

        void onExtentChanged();

      protected:

        te::map::AbstractLayerPtr m_layer;
        Feature* m_feature;

    //QList<Feature*> m_feature;// m_tools;

    };

  }   // end namespace edit
}     // end namespace te

#endif  // __TERRALIB_EDIT_QT_INTERNAL_DELETEGEOMETRYTOOL_H
