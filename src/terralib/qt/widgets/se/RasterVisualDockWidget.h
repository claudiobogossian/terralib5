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
  \file terralib/qt/widgets/se/RasterVisualDockWidget.h

  \brief A dock widget used control the raster enhancement
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_RASTERVISUALDOCKWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_RASTERVISUALDOCKWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QDockWidget>

namespace te
{
  namespace map { class DataSetLayer; }
  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class RasterVisualWidget;


      /*!
        \class RasterVisualDockWidget

        \brief A dock widget used control the raster enhancement
      */
      class TEQTWIDGETSEXPORT RasterVisualDockWidget : public QDockWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a RasterVisualDockWidget. */
          RasterVisualDockWidget(const QString & title, QWidget * parent = 0, Qt::WindowFlags flags = 0);

          /*! \brief Destructor. */
          ~RasterVisualDockWidget();

          //@}

          void setLayer(te::map::DataSetLayer* layer);

        protected:

          /*! \brief Updates the widget form based on internal mark element. */
          void updateUi();

        protected slots:

          void onSymbolizerUpdated();

        signals:

          void symbolizerChanged();

        private:

          te::qt::widgets::RasterVisualWidget* m_visualWidget;   //!< Visual widget.
          te::map::DataSetLayer* m_layer;                        //!< DataSetLayer layer pointer;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_RASTERVISUALDOCKWIDGET_H
