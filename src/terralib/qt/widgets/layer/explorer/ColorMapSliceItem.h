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
 * \file ColorMapSliceItem.h
 *
 \brief A class that represents a grouping item of a color map in a LayerTreeModel.
 */

#ifndef __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_COLORMAPSLICEITEM_H
#define __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_COLORMAPSLICEITEM_H

#include "TreeItem.h"

// Qt
#include <QColor>
#include <QIcon>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
      * \class ColorMapSliceItem
      *
      * \brief A class that represents a grouping item of a color map in a LayerTreeModel.
      *
      * \note The type of the item is "COLORMAPSLICE".
      */
      class TEQTWIDGETSEXPORT ColorMapSliceItem: public TreeItem
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
        ColorMapSliceItem(const std::string& label, const QColor& beg, const QColor& end = QColor());

        /*!
        * \brief Destructor.
        */
        ~ColorMapSliceItem();
        //@}

        /*!
        * \name Reimplementation of TreeItem
        *
        * Methods reimplemented of the TreeItem class.
        */
        //@{
        std::string getAsString() const;
        //@}

        QIcon getIcon() const;

      protected:

        std::string m_label;    //!< Label to be presented on the Qt view.

        QIcon m_icon;
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_LAYER_INTERNAL_COLORMAPSLICEITEM_H
