/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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

\brief 
\version 5.0
\author Frederico Augusto Bedê <frederico.bede@funcate.org.br>
\date 01/03/2011
*/
#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_HIGHLIGHTDELEGATE_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_HIGHLIGHTDELEGATE_H

#include <terralib/qt/widgets/Config.h>

//Qt4 include files
#include <QStyledItemDelegate>
#include <QObject>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QColor>

//STL include files
#include <set>
#include <string>
#include <vector>

namespace te 
{
  namespace qt 
  {
    namespace widgets 
    {

      /*!
        \class HighlightDelegate
        \brief 
      */
      class TEQTWIDGETSEXPORT HighlightDelegate : public QStyledItemDelegate 
      {
        /** @name Initializer methods.
        *  Methods related to instantiation and destruction.
        */
        //@{

      public:
        /*! 
          \brief Constructor.    
          \param parent Qt parent.
        */
        HighlightDelegate(QObject * parent = 0);

        /*! 
          \brief Virtual destructor.
        */
        virtual ~HighlightDelegate();
        //@}

        /** @name QStyledItemDelegate re-implementation methods.
        *  Re-implementation of QStyledItemDelegate methods.
        */
        //@{

        /*! 
          \overload Over void paint(::QPainter * painter, const ::QStyleOptionViewItem & option, const ::QModelIndex & index) const
        */
        virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
        //@}

        /** @name Setup methods.
        *  Methods used to set up the internal members.
        */
        //@{

        void setEnabled(const bool& enable);

        /*!
          \brief Updates the set of objects to be highlighted.    

          Updates the set of selected objects. Note that if exists an older selection, it will be removed.

          \param value The new set of selected objects.
        */
        void setHighlightedPkeys(const std::set<std::string> & value);

        /*! 
          \brief Update color used to highlight a row.    
        */
        void setHighlightColor(const QColor & value);

        /*! 
          \brief Returns the color being used.
          \return Color being used.
        */
        QColor getHighlightColor() const;

        /*! 
          \brief Updates the set of primary keys.    
          \param pKeyCols Columns positions.
        */
        virtual void setPKeysColumns(const std::vector<size_t> & pKeyCols);

        /*! 
          \brief Returns the vector containing the positions of the primary keys.    
          \return The vector containing the positions of the primary keys.
        */
        std::vector<size_t> getPKeysColumns();

        /*! 
          \brief Given an index returns its primary key.    
        */
        std::string getPKey(const QModelIndex & index, char separator = '#') const;

        /*! 
          \brief Returns a vector contained the ids of the highlighted objects.    
          \return Vector with the ids for the highlighted objects.
        */
        std::set<std::string> getHighlightKeys();

        /*!
          \brief Removes all selections.
        */
        virtual void clearSelection();

        /*!
          \brief Updates group name. The group name is used only for presentation on GUI.
          \param name New name of the group.
        */
        void setGroupName(const QString& name);

        /*!
          \brief Returns the group name.
          \return Group name.
        */
        QString getGroupName() const;
        //@}

      protected:
        std::vector<size_t> m_pkeys;    //!< Primary keys positions.    
        std::set<std::string> m_hlOids; //!< Set of primary keys of the data set.    
        QColor m_color;                 //!< Color to be used for paint selected keys.
        bool m_enabled;                 //!< Enable or disable.
        QString m_grp_name;             //!< Name of the group (for presentation purposes).

        /** @name Copy methods.
        *  No copy allowed.
        */
        //@{

      private:
        /*! 
          \brief Copy constructor.    
        */
        HighlightDelegate(const HighlightDelegate & source);

        /*! 
          \brief Copy operator.    
        */
        HighlightDelegate & operator=(const HighlightDelegate & source);
        //@}
      };
    } // namespace te::qt::widgets
  } // namespace te::qt
} // namespace te

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_HIGHLIGHTDELEGATE_H
