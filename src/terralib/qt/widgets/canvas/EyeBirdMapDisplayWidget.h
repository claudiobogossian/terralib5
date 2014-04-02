/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/canvas/EyeBirdMapDisplayWidget.h

  \brief This file has the EyeBirdMapDisplayWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_EYEBIRDMAPDISPLAYWIDGET_H
#define __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_EYEBIRDMAPDISPLAYWIDGET_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWidget>

namespace te
{
  namespace gm { class Envelope; }

  namespace qt
  {
    namespace widgets
    {
      class MapDisplay;
      class PanExtent;

      /*!
        \class EyeBirdMapDisplayWidget

        \brief This class defines the map display EyeBird, this component is only a specific 
               map that shows the original extent and a box representing the current extent.
      */
      class TEQTWIDGETSEXPORT EyeBirdMapDisplayWidget : public QWidget
      {
        Q_OBJECT

        public:

          EyeBirdMapDisplayWidget(te::qt::widgets::MapDisplay* mapParent, QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~EyeBirdMapDisplayWidget();

        public:

          /*!
            \brief This method is used to set the selected layer for mixture model operation
            
            \param layer The layer ptr 

            \note This layer MUST HAVE a valid raster object.
          */
          void setList(std::list<te::map::AbstractLayerPtr>& layerList, int srid);

          void recompose(te::gm::Envelope& e);

          void setEnabled(bool status);

        protected slots:

          void onExtentMoved(te::gm::Envelope e);

          void onParentMapDisplayExtentChanged();

        private:

          te::gm::Envelope m_inputExt;

          te::qt::widgets::MapDisplay* m_mapDisplay;

          te::qt::widgets::MapDisplay* m_parentMapDisplay;

          te::qt::widgets::PanExtent* m_panExtent;

          bool m_itsMe;
          bool m_isEnabled;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_EYEBIRDMAPDISPLAYWIDGET_H

