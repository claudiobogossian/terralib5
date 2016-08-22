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
  \file terralib/edit/qt/tools/SubtratAreaTool.cpp

  \brief This class implements a concrete tool to subtract geometries.
*/

#ifndef __TERRALIB_EDIT_QT_INTERNAL_SUBTRACTAREATOOL_H
#define __TERRALIB_EDIT_QT_INTERNAL_SUBTRACTAREATOOL_H

// TerraLib
#include "../../../geometry/Envelope.h"
#ifndef Q_MOC_RUN
#include "../../../maptools/AbstractLayer.h"
#endif
#include "../core/command/AddCommand.h"
#include "../core/UndoStackManager.h"
#include "../Config.h"

// Qt
#include <QPointF>

#ifndef Q_MOC_RUN
#include "CreateLineTool.h"
#endif

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
      SubtractAreaTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, const te::edit::MouseEventEdition mouseEventToSave, QObject* parent = 0);

      ~SubtractAreaTool();

      //@}

      /** @name AbstractTool Methods
      *  Methods related with tool behavior.
      */
      //@{

      bool mousePressEvent(QMouseEvent* e);

      bool mouseMoveEvent(QMouseEvent* e);

      bool mouseDoubleClickEvent(QMouseEvent* e);

      bool mouseReleaseEvent(QMouseEvent* e);

      //@}

    private:

      bool subtractPolygon();

      void drawPolygon();

      void draw();

      void storeFeature();

      void storeUndoCommand();

      void pickFeature(const te::map::AbstractLayerPtr& layer, const QPointF& pos);

      te::gm::Envelope buildEnvelope(const QPointF& pos);

      te::gm::Geometry* buildPolygon();

      te::gm::Geometry* differenceGeometry(te::gm::Geometry* g1, te::gm::Geometry* g2);

    protected:

      std::set<std::string> m_oidsSet;
      MouseEventEdition m_mouseEventToSave;
      UndoStackManager& m_stack;

    };

  }   // end namespace edit
}     // end namespace te

#endif  // __TERRALIB_EDIT_QT_INTERNAL_AGGREGATEAREATOOL_H
