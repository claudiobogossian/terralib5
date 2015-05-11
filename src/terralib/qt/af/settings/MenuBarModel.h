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
    namespace af
    {

      /*!
        \class MenuBarModel

        \brief Defines a model, based on a QMenuBar, to present all system buttons as a table.
      */
      class MenuBarModel : public QAbstractTableModel
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.

            \param mnuBar The menu bar to be used.

            \param parent Parent object.
          */
          MenuBarModel(QMenuBar* mnuBar, QObject* parent=0);

          /*!
            \brief Destructor.
          */
          ~MenuBarModel();

          virtual Qt::ItemFlags	flags (const QModelIndex & index) const;      
          
          QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
          
          QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
      
          int columnCount (const QModelIndex & parent = QModelIndex()) const;
          
          int rowCount (const QModelIndex & parent = QModelIndex()) const;

          bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

          /*!
            \brief Check the actions contained in \a acts.

            \param acts List of the actions.
          */
          void updateActionsState(const QList<QAction*>& acts);

          /*!
            \brief Sets the menu bar to be used in the model.
          */
          void setMenuBar(QMenuBar* mnuBar);

        signals:

          void updateAction(QAction*, const bool&);

        protected:

          std::vector<QAction*> m_actions;        //!< System buttons.
          std::vector<bool> m_actionsActivation;  //!< State of each system button. (True insert, False do nothing).
     };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_MENUBARMODEL_H
