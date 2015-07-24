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
 * \file FolderItem.h
 *
 * \brief Defines a folder item, just containing LayerItem.
 */
#ifndef __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_FOLDERITEM_H
#define __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_FOLDERITEM_H

#include "TreeItem.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
      * \class FolderItem
      *
      * \brief Represents a folder item that contains layers and / or other folders.
      *
      * \note The type of the item is "FOLDER".
      */
      class TEQTWIDGETSEXPORT FolderItem: public TreeItem
      {
      public:
        /*!
        * \name Instantiation Methods
        *
        * Methods related to creation and destruction of the object.
        */
        //@{

        /*!
        * \brief Constructor.
        *
        * \param label Label to be presented in the Qt view.
        */
        FolderItem(const std::string& label);

        /*!
        * \brief Destructor.
        */
        ~FolderItem();
        //@}

        /*!
        * \name Reimplementation of TreeItem
        *
        * Methods reimplemented of the TreeItem class.
        */
        //@{
        std::string getAsString() const;

        VISIBLE isVisible() const;

        void setVisible(const VISIBLE& visible, const bool& updateAncestors = false, const bool& updateDescendents = false);
        //@}

        void setTitle(const std::string& title);

      protected:
        std::string m_label;    //!< Label to be presented on the Qt view.
        VISIBLE m_v;            //!< Visibility of the folder.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_LAYER_INTERNAL_FOLDERITEM_H
