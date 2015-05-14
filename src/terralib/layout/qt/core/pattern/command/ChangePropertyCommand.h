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
  \file ChangePropertyCommand.h
   
  \brief Undo/Redo for changes in component properties.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_CHANGE_PROPERTY_COMMAND_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_CHANGE_PROPERTY_COMMAND_ITEM_H

// TerraLib
#include "../../../../core/Config.h"

// STL
#include <vector>

// Qt
#include <QUndoCommand>

class QGraphicsItem;

namespace te
{
  namespace layout
  {
    class Properties;
    class PropertiesOutside;

    /*!
      \brief Undo/Redo for changes in component properties.
	  
	    \ingroup layout
	  */
    class TELAYOUTEXPORT ChangePropertyCommand : public QUndoCommand
    {
      public:

        /*!
          \brief Constructor

          \param item
          \param oldProperties
          \param newProperties
          \param outside
          \param parent
        */
        ChangePropertyCommand( QGraphicsItem* item, Properties* oldProperties, 
                          Properties* newProperties, PropertiesOutside* outside = 0, QUndoCommand *parent = 0 );

        /*!
          \brief Constructor

          \param items
          \param oldProperties
          \param newProperties
          \param outside
          \param parent
        */
        ChangePropertyCommand( std::vector<QGraphicsItem*> items, std::vector<Properties*> allOld, 
                    std::vector<Properties*> allNew, PropertiesOutside* outside = 0, QUndoCommand *parent = 0 );

        /*!
          \brief Destructor. Delete all properties.
        */
        virtual ~ChangePropertyCommand();   

        /*!
          \brief Reimplemented from QUndoCommand
        */
        virtual void undo();

        /*!
          \brief Reimplemented from QUndoCommand
        */
        virtual void redo();

      protected:

        virtual QString createCommandString(QGraphicsItem* item);

        virtual bool equals(Properties*  props1, Properties* props2);

        virtual bool checkItem(QGraphicsItem* item, Properties* props); 

        virtual bool checkVectors();

        QGraphicsItem*                  m_item;
        Properties*                     m_oldProperties;
        Properties*                     m_newProperties;
        PropertiesOutside*              m_outside;
        std::vector<QGraphicsItem*>     m_items;
        std::vector<Properties*>        m_allOldProperties;
        std::vector<Properties*>        m_allNewProperties;
    };
  }
}

#endif
