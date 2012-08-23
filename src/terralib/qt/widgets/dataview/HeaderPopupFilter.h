/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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
  \file HeaderPopupFilter.h
  \brief 
  \details 
  \version 5.0
  \author Frederico Augusto Bed&ecirc; &lt;frederico.bede@funcate.org.br&gt;
  \date 2001-2012
  \copyright GNU Lesser General Public License.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_HEADERPOPUPFILTER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_HEADERPOPUPFILTER_H

#include <terralib/qt/widgets/Config.h>

//Qt include files
#include <QObject>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class HeaderPopupFilter
        \copydoc HeaderPopupFilter.h
      */
      class TEQTWIDGETSEXPORT HeaderPopupFilter : public QObject
      {
        Q_OBJECT

      public:

        HeaderPopupFilter(QObject* parent=0);

        ~HeaderPopupFilter();

      protected:

      };
    }
  }
}


#endif //__TERRALIB_QT_WIDGETS_INTERNAL_HEADERPOPUPFILTER_H
