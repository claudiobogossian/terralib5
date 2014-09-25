/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file DeleteCommand.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_DELETE_COMMAND_H 
#define __TERRALIB_LAYOUT_INTERNAL_DELETE_COMMAND_H

// Qt
#include <QUndoCommand>
#include <QString>
#include <QPointF>

class QGraphicsItem;
class QGraphicsScene;

namespace te
{
  namespace layout
  {
    class DeleteCommand : public QUndoCommand
    {
      public:

        DeleteCommand(QGraphicsScene* scene, QUndoCommand *parent = 0);

        virtual ~DeleteCommand();           

        virtual void undo();

        virtual void redo();

      protected:

        virtual QString createCommandString(int totalItems);

        QGraphicsScene* m_scene;
        QList<QGraphicsItem*> m_items;
    };
  }
}

#endif
