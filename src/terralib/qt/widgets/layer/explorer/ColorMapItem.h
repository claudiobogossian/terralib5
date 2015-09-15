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
 * \file ColorMapItem.h
 *
 \brief A class that represents a color map of rastersymbolizer of a layer in a LayerItemModel.
 */
#ifndef __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_COLORMAPITEM_H
#define __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_COLORMAPITEM_H

#include "TreeItem.h"

namespace te
{
  // Forward declarations
  namespace se
  {
    class ColorMap;
  }

  namespace qt
  {
    namespace widgets
    {
      /*!
      * \class ColorMapItem
      *
      * \brief Represents a color map of rastersymbolizer of a layer in a LayerItemModel.
      *
      * \note The type of the item is "COLORMAP".
      *
      * \ingroup widgets
      */
      class TEQTWIDGETSEXPORT ColorMapItem: public TreeItem
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
        * \param map Color map.
        */
        ColorMapItem(const te::se::ColorMap* map);

        /*!
        * \brief Destructor.
        */
        ~ColorMapItem();
        //@}

        /*!
        * \name Reimplementation of TreeItem
        *
        * Methods reimplemented of the TreeItem class.
        */
        //@{
        std::string getAsString() const;
        //@}

      protected:

        std::string m_label;                  //!< Label to be presented on the Qt view.
        const te::se::ColorMap* m_colorMap;   //!< ColorMap being used.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_LAYER_INTERNAL_COLORMAPITEM_H
