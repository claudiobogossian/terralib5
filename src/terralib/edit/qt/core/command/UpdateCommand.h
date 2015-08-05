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
\file UpdateCommand.h

\brief

\ingroup edit
*/

#ifndef __TERRALIB_EDIT_QT_INTERNAL_UPDATE_COMMAND_H 
#define __TERRALIB_EDIT_QT_INTERNAL_UPDATE_COMMAND_H

// TerraLib
#include "../../../../qt/widgets/canvas/MapDisplay.h"
#include "../../Config.h"
#include "../EditionManager.h"
#include "../../../Feature.h"

// Qt
#include <QUndoCommand>
#include <QString>
#include <QPointF>

namespace te
{
  namespace edit
  {
    /*!
    \brief Undo/Redo for Update one components.

    */
    class TEEDITQTEXPORT UpdateCommand : public QUndoCommand
    {
      public:
        // Pass all parameters that the command need to fulfill its task in the constructor
        UpdateCommand(te::edit::EditionManager* editionManager, std::vector<Feature*> items, te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QUndoCommand *parent = 0);

        /*!
        \brief Destructor
        */
        virtual ~UpdateCommand();

        /*!
        \brief Reimplemented from QUndoCommand
        */
        virtual void undo();

        /*!
        \brief Reimplemented from QUndoCommand
        */
        virtual void redo();

      private:

        virtual QString createCommandString(QString oid);

        void draw();

        te::qt::widgets::MapDisplay* m_display;
        te::map::AbstractLayerPtr m_layer;
        std::vector<Feature*> m_updateItems;
        te::edit::EditionManager* m_editionManager;
        te::gm::Coord2D    m_initialPosition;

        std::size_t m_pos;

    };
  }
}

#endif