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
  \file terralib/qt/widgets/rp/RasterHistogramDialog.h

  \brief This file defines a class for a RasterHistogram Dialog.
*/

#ifndef __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_RASTERHISTOGRAMDIALOG_H
#define __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_RASTERHISTOGRAMDIALOG_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QDialog>

// Forward declaraion
namespace Ui { class RasterHistogramDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class RasterHistogramWidget;

      /*!
        \class RasterHistogramDialog

        \brief This class is a dialog for the RasterHistogram widget.
      */
      class TEQTWIDGETSEXPORT RasterHistogramDialog : public QDialog
      {
        Q_OBJECT

        public:

          RasterHistogramDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~RasterHistogramDialog();

        public:

          te::qt::widgets::RasterHistogramWidget* getWidget();

          void set(te::map::AbstractLayerPtr layer);

        private:
          std::auto_ptr<Ui::RasterHistogramDialogForm> m_ui;
          std::auto_ptr<te::qt::widgets::RasterHistogramWidget> m_widgetRasterHistogram;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_RASTERHISTOGRAMDIALOG_H
