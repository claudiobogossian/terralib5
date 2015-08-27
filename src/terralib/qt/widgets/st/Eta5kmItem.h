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
\file terralib/qt/widgets/st/Eta5kmItem.h

\brief This file defines a class for a Eta5kmItem.
*/

#ifndef __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_ETA5KM_ITEM_H
#define __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_ETA5KM_ITEM_H

// TerraLib
#include "ImageItem.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
      \class This class is used to make animation of Eta5km forecast image on map display.
      */
      class TEQTWIDGETSEXPORT Eta5kmItem : public ImageItem
      {
      public:

        /*!
        \brief Empty Constructor.
        */
        Eta5kmItem();

        /*!
        \brief Constructor
        It constructs a Eta5km Item.
        \param title The item title.
        \param file The absolute file path name.
        \param display Where the item is displayed.
        */
        Eta5kmItem(const QString& title, const QString& file, te::qt::widgets::MapDisplay* display);

        /*!
        \brief Destructor
        It destructs a Eta5km Item.
        */
        virtual ~Eta5kmItem();

      private:

        /*!
        \brief Load temporal raster data.

        \Return True if success.
        */
        bool loadData();

        /*!
        \brief Get control parameters.
        */
        bool getCtlParameters();

        /*!
        \brief Load current image.

        \Return pointer to current image.
        */
        void loadCurrentImage();

        /*!
        \brief Get time of the raster data.

        \Param file raster data file name.

        \Return The time instant.
        */
        te::dt::TimeInstant getTime(QString file);

        /*!
        \brief Sets the default LUT.
        */
        void setLUT();
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_ETA5KM_ITEM_H
