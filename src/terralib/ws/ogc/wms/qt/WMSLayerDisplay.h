/*  Copyright (C) 2001-2016 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/ws/ogc/wms/qt/WMSLayerDisplay.h

  \brief A display QFrame to draw previews for WMS data.

  \author Emerson Moraes
*/

#ifndef __TERRALIB_WS_OGC_WMS_QT_WMSLAYERDISPLAY_H
#define __TERRALIB_WS_OGC_WMS_QT_WMSLAYERDISPLAY_H

#include "Config.h"

//TerraLib
#include "../../../../qt/widgets/canvas/Canvas.h"
#include "../client/DataTypes.h"

// Qt
#include <QFrame>

// STL
#include <memory>

namespace te
{
  namespace ws
  {
    namespace ogc
    {
      namespace wms
      {
        namespace qt
        {

        /*!
          \class WMSLayerDisplay

          \brief A map display for a WMSGetMapResponse.
        */
        class TEOGCWMSQTEXPORT WMSLayerDisplay : public QFrame
        {
          Q_OBJECT

        public:

          WMSLayerDisplay(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~WMSLayerDisplay();

          /*!
           * \brief Draws a WMSGetMapResponse in the preview Canvas.
           * \param response WMSGetMapResponse to draw.
           */
          void draw(const te::ws::ogc::wms::WMSGetMapResponse& response);

          void clear();

        protected:

          void paintEvent(QPaintEvent* e);

          void resizeEvent(QResizeEvent* e);

        private:

          std::unique_ptr<te::qt::widgets::Canvas> m_canvas;
        };

        }
      }
    }
  }
}

#endif // __TERRALIB_WS_OGC_WMS_QT_WMSLAYERDISPLAY_H
