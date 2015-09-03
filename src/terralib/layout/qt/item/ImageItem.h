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
  \file ImageItem.h
   
   \brief Class that represents a graphic Image. 
      Its coordinate system is the same of scene (millimeters). 
      This is also son of ItemObserver and ObjectItem, so it can become observer of a model (Observable).  

  \ingroup layout
*/
#ifndef __TERRALIB_LAYOUT_INTERNAL_IMAGE_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_IMAGE_ITEM_H

// TerraLib
#include "AbstractItem.h"
#include "../../core/Config.h"

#include <QGraphicsItem>

namespace te
{
  namespace layout
  {
    class AbstractItemController;
    /*!
    \brief Class that represents a graphic Arrow. 
        Its coordinate system is the same of scene (millimeters). 
        He is also the son of AbstractItem, so it can become observer of a model (Observable). 
    
      \ingroup layout

      \sa te::layout::ObjectItem
    */
    class TELAYOUTEXPORT ImageItem : public AbstractItem<QGraphicsItem>
    {
      public:

        /*!
        \brief Constructor

        \param controller "Controller" part of MVC component
        \param o "Model" part of MVC component
        */
        ImageItem(AbstractItemController* controller);

        /*!
        \brief Destructor
        */
        virtual ~ImageItem();

        /*!
        \brief Gets the file name of the image
        */
        virtual const std::string& getFileName() const;

        /*!
        \brief Sets the file name of the image
        */
        virtual void setFileName(const std::string& fileName);

      protected:

        virtual void drawItem(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

      protected:

        QImage      m_image; //!< Represents the loaded image
        std::string m_fileName; //!< The file name of the loaded image
    };
  }
}

#endif
