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
  \file MenuBarModel.h

  \brief Contains a definition for a model over the QMenuBar.
*/
#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_MENUBARMODEL_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_MENUBARMODEL_H

#include <QtCore/QAbstractTableModel>

//Forward declaration
class QMenuBar;
class QToolBar;
class QAction;

//STL
#include <vector>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class MenuBarModel : public QAbstractTableModel
      {
        public:

          MenuBarModel(QMenuBar* mnuBar, QObject* parent=0);

          ~MenuBarModel();

          virtual Qt::ItemFlags	flags (const QModelIndex & index) const;      
          
          QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
          
          QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
      
          int columnCount (const QModelIndex & parent = QModelIndex()) const;
          
          int rowCount (const QModelIndex & parent = QModelIndex()) const;

          bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

          void updateActionsState(QToolBar* toolBar);

          /*!
            \brief Returns a vector with all checked actions.
          */
          std::vector<QAction*> getActions() const;

          void setMenuBar(QMenuBar* mnuBar);

        protected:

          std::vector<QAction*> m_actions;
          std::vector<bool> m_actionsActivation;
     };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_MENUBARMODEL_H
