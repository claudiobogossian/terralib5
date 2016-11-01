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
\file terralib/qt/widgets/utils/GeoFileDragAndDropHandler.h

\brief A class used to handle geofile drag and drop events.
*/

#ifndef __TE_QT_PLUGINS_DATASOURCE_GDAL_INTERNAL_GEOFILEDRAGANDDROPHANDLER_H
#define __TE_QT_PLUGINS_DATASOURCE_GDAL_INTERNAL_GEOFILEDRAGANDDROPHANDLER_H

// TerraLib
#include "Config.h"

// Qt
#include <QObject>

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace gdal
      {
        /*!
        \class GeoFileDragAndDropHandler

        \brief A class used to handle geofile drag and drop events.

        \ingroup widgets
        */
        class TEQTPLUGINGDALEXPORT GeoFileDragAndDropHandler : public QObject
        {
          Q_OBJECT

          public:

            GeoFileDragAndDropHandler();

            ~GeoFileDragAndDropHandler();

          public:

            virtual bool eventFilter(QObject* watched, QEvent* e);

          protected:

            bool m_isValid;
        };

      } // end namespace ogr
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

#endif  // __TE_QT_PLUGINS_DATASOURCE_GDAL_INTERNAL_GEOFILEDRAGANDDROPHANDLER_H

