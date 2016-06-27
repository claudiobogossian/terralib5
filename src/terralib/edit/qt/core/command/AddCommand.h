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
\file AddCommand.h

\brief

\ingroup edit
*/

#ifndef __TERRALIB_EDIT_QT_INTERNAL_ADD_COMMAND_H 
#define __TERRALIB_EDIT_QT_INTERNAL_ADD_COMMAND_H

// TerraLib
#include "../../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../../dataaccess/dataset/ObjectId.h"
//#include "../../../Feature.h"
#include "../../../RepositoryManager.h"
#include "../../../Utils.h"
#include "../../Config.h"
#include "../../Renderer.h"
#include "../../Utils.h"
#include "../UndoStackManager.h"

// Qt
#include <QUndoCommand>

namespace te
{
  namespace edit
  {
    /*!
    \brief Undo/Redo for add one components.

    */
    class TEEDITQTEXPORT AddCommand : public QObject, public QUndoCommand
    {
      Q_OBJECT

      public:
        // Pass all parameters that the command need to fulfill its task in the constructor
        AddCommand(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, te::da::ObjectId* id, QUndoCommand *parent = 0);

        /*!
        \brief Destructor
        */
        virtual ~AddCommand();

        /*!
        \brief Reimplemented from QUndoCommand
        */
        virtual void undo();

        /*!
        \brief Reimplemented from QUndoCommand
        */
        virtual void redo();

      signals:

        void undoFeedback(std::vector<te::gm::Coord2D> coords);

      private:

        te::qt::widgets::MapDisplay* m_display;
        te::map::AbstractLayerPtr m_layer;
        te::da::ObjectId* m_id;
        UndoStackManager& m_stack;

        void draw();

        std::size_t countFeaturesById(te::da::ObjectId* id, std::size_t& lastPos);

    };
  }
}

#endif