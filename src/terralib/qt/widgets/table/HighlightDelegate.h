/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file HighlightDelegate.h

  \brief A delegate for highlight the selected object ids.
*/


#ifndef __TERRALIB_QT_WIDGETS_TABLE_INTERNAL_DATASETTABLEVIEW_H
#define __TERRALIB_QT_WIDGETS_TABLE_INTERNAL_DATASETTABLEVIEW_H

#include "../Config.h"

// Qt 
#include <QItemDelegate>

namespace te
{
  namespace da
  {
    class ObjectIdSet;

    class ObjectId;
  }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class HighlightDelegate

        \brief An specialization of QItemDelegate to be used with te::map::AbstractTable objects.
        
        This class is used on QAbstractItemView that has a te::qt::widgets::DatSetModel as model to highlight some objects. 
        Each identifier in the set of ids are rendering with a predefined color.
        
        \sa QItemDelegate, te::qt::widgets::DataSetTableModel
       */
      class TEQTWIDGETSEXPORT HighlightDelegate : public QItemDelegate 
      {
        public:

          /*!
            \brief Constructor.

            \param parent QObject parent.
          */
          HighlightDelegate(QObject* parent=0);

          /*!
            \brief Destructor
          */
          virtual ~HighlightDelegate();

          /*!
            \brief Update the color group.

            \param c New color of the group.
          */
          virtual void setColor(const QColor& c);

          /*!
            \brief Returns the color.

            \return Current color.
          */
          QColor getColor();

          /*!
            \brief Adds the objects in the group.

            \param objs The objects to be inserted.
          */
          virtual void addObjects(te::da::ObjectIdSet* objs);

        protected:

          QColor m_color;               //!< Highlight color.

          te::da::ObjectIdSet* m_objs;  //!< Identifiers of the highlight object.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_TABLE_INTERNAL_DATASETTABLEVIEW_H
