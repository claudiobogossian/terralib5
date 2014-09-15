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
  \file ChangePropertyCommand.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_CHANGE_PROPERTY_COMMAND_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_CHANGE_PROPERTY_COMMAND_ITEM_H

// Qt
#include <QUndoCommand>

class QGraphicsItem;

namespace te
{
  namespace layout
  {
    class Properties;
    class PropertiesOutside;

    class ChangePropertyCommand : public QUndoCommand
    {
      public:

        ChangePropertyCommand( QGraphicsItem* item, Properties* oldProperties, 
                          Properties* newProperties, PropertiesOutside* outside = 0, QUndoCommand *parent = 0 );

        virtual ~ChangePropertyCommand();   

        virtual void undo();

        virtual void redo();

      protected:

        virtual QString createCommandString(QGraphicsItem* item);

        virtual bool equals(Properties*  props1, Properties* props2);

        QGraphicsItem*     m_item;
        Properties*        m_oldProperties;
        Properties*        m_newProperties;
        PropertiesOutside* m_outside;
    };
  }
}

#endif