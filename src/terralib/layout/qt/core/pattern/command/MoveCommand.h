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
  \file MoveCommand.h
   
  \brief Undo/Redo for moving components.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MOVE_COMMAND_H 
#define __TERRALIB_LAYOUT_INTERNAL_MOVE_COMMAND_H

// TerraLib
#include "../../../../core/Config.h"

// Qt
#include <QUndoCommand>
#include <QString>
#include <QPointF>

// STL
#include <map>
#include <vector>

class QGraphicsItem;

namespace te
{
  namespace layout
  {
    /*!
      \brief Undo/Redo for moving components.
	  
	    \ingroup layout
	  */
    class TELAYOUTEXPORT MoveCommand : public QUndoCommand
    {
      public:
        
        /*!
          \brief Constructor

          \param item
          \param oldPos
          \param parent
        */
        MoveCommand(QGraphicsItem* item, const QPointF oldPos,
          QUndoCommand *parent = 0);

        /*!
          \brief Constructor

          \param items
          \param parent
        */
        MoveCommand(std::map<QGraphicsItem*, QPointF> items, QUndoCommand *parent = 0);

        /*!
          \brief Destructor
        */
        virtual ~MoveCommand();       

        /*!
          \brief Reimplemented from QUndoCommand
        */
        virtual void undo();

        /*!
          \brief Reimplemented from QUndoCommand
        */
        virtual void redo();
        
      protected:

        virtual QString createCommandString(QGraphicsItem* item, const QPointF &pos);

        QGraphicsItem*  m_item;
        QPointF         m_oldPos;
        QPointF         m_newPos;
        std::map<QGraphicsItem*, QPointF> m_moveItems;
        QList<QPointF>              m_itemsPoints;
    };
  }
}

#endif
