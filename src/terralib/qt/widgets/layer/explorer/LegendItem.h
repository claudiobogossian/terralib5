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
 * \file layer/explorer/LegendItem.h
 *
 * \brief Defines a legend item.
 */
#ifndef __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LEGENDITEM_H
#define __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LEGENDITEM_H

#include "TreeItem.h"

// Qt
#include <QIcon>

// Forward declarations
namespace te
{
  namespace se
  {
    class Rule;
    class Symbolizer;
  }

  namespace qt
  {
    namespace widgets
    {
      /*!
      * \class LegendItem
      *
      * \brief Represents a lengend in the tree structure.
      *
      * \note The type of the item is "LEGEND".
      */
      class TEQTWIDGETSEXPORT LegendItem: public TreeItem
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
        * \param rule The Rule that defines the legend item.
        */
        LegendItem(const te::se::Rule* rule);

        /*!
        * \brief Constructor.
        *
        * \param label The label to be presented.
        *
        * \param symbolizers List of symbolizers used to create the icon.
        */
        LegendItem(const std::string& label, const std::vector<te::se::Symbolizer*>& symbolizers);

        /*!
        * \brief Destructor.
        */
        ~LegendItem();
        //@}

        /*!
        * \name Reimplementation of TreeItem
        *
        * Methods reimplemented of the TreeItem class.
        */
        //@{
        std::string getAsString() const;
        
        Qt::ItemFlags flags();
        //@}

        /*!
        * \brief Returns the image of the style as an icon.
        *
        * \return The style as an icon.
        */
        QIcon getIcon() const;

      protected:

        std::string m_label;    //!< Label for presenting in the Qt view.
        QIcon m_icon;           //!< Item icon.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LEGENDITEM_H
