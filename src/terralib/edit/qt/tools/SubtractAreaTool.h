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
  \file terralib/edit/qt/tools/SubtractAreaTool.h

  \brief This class implements a concrete tool to subtract geometries.
*/

#ifndef __TERRALIB_EDIT_QT_INTERNAL_SUBTRACTAREATOOL_H
#define __TERRALIB_EDIT_QT_INTERNAL_SUBTRACTAREATOOL_H

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// Qt
#include <QPointF>

#include "CreateLineTool.h"

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

    class TEEDITQTEXPORT SubtractAreaTool : public CreateLineTool
    {
      Q_OBJECT

    public:
      SubtractAreaTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent = 0);

      ~SubtractAreaTool();

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

      void drawPolygon();
      void draw();
      void reset();
      void storeEditedFeature();

      void pickFeature(const te::map::AbstractLayerPtr& layer, const QPointF& pos);

      te::gm::Envelope buildEnvelope(const QPointF& pos);

      te::gm::Geometry* buildPolygon();

      te::gm::Geometry* Difference(te::gm::Geometry* g1, te::gm::Geometry* g2);

      private slots:

      void onExtentChanged();

    protected:

      Feature* m_feature;

    };

  }   // end namespace edit
}     // end namespace te

#endif  // __TERRALIB_EDIT_QT_INTERNAL_SUBTRACTAREATOOL_H