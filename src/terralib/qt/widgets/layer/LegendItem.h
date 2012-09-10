/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file LegendItem.h

  \brief A widget designed to show the information of a LegendItem in a tree oriented way.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_LEGENDITEM_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_LEGENDITEM_H

// TerraLib
#include "../Config.h"
#include "AbstractTreeItem.h"

// Qt
#include <QtCore/QVariant>
#include <QtGui/QIcon>

// Forward declarations
class QMenu;
class QWidget;


namespace te
{
  namespace map { class LegendItem; }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class LegendItem

        \brief A widget designed to show the information of a legend in a tree oriented way.
       */
      class TEQTWIDGETSEXPORT LegendItem : public AbstractTreeItem
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It initializes a legend item with a reference to the parent layer item

            \param parent The parent of this legend item

           */
          LegendItem(te::map::LegendItem* refLegendItem, QObject* parent = 0);

          /*! Destructor. */
          ~LegendItem();

          /*!
            \brief It checks if the item is a layer item.

            \return It returns false indicating that the item is not a layer item.
           */
          bool isLayerItem() const;

          /*!
            \brief It checks if the item is a legend item.

            \return It returns true indicating that the item is a legend item.
           */
          bool isLegendItem() const;

          //@}

          /** @Methods that subclasses must implement.
           */
          //@{

          /*!
            \brief It returns the data stored under the given role for the item referred by this layer.

            This method will be called by the implementation of LayerExplorerModel.

            \param role The role are used by a given view to indicate to this model which type of data it needs.

            \return The data stored under the given role for the item referred by this layer.
           */
          QVariant data(int role) const;

          /*!
            \brief It creates a menu to be displayed in the given widget.

            \param parent The parent widget for the menu.

            \return A menu to be displayed in the given widget.
           */
          QMenu* getMenu(QWidget* parent = 0) const;

          /*!
            \brief It gets the icon that is associated to the legend item.

            \return The icon that is associated to the legend item.
           */
          const QIcon& getIcon() const;

          /*!
            \brief It sets the icon to be associated to the legend item.

            \param icon The icon to be associated to the legend item.
           */
          void setIcon(const QIcon& icon);

          //@}

      private:
        te::map::LegendItem* m_refLegendItem;
        QIcon m_icon;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_LEGENDITEM_H
