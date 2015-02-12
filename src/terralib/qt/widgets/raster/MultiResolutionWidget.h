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
  \file terralib/qt/widgets/raster/MultiResolutionWidget.h

  \brief A widget to create multi resolution over a raster.
*/

#ifndef __TERRALIB_QT_WIDGETS_RASTER_INTERNAL_MULTIRESOLUTIONWIDGET_H
#define __TERRALIB_QT_WIDGETS_RASTER_INTERNAL_MULTIRESOLUTIONWIDGET_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWidget>

namespace Ui { class MultiResolutionWidgetForm; }

namespace te
{
  //forward declaration
  namespace rst { class Raster; }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class MultiResolutionWidget

        \brief A widget to create multi resolution over a raster.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT MultiResolutionWidget : public QWidget
      {
        Q_OBJECT

        public:

          MultiResolutionWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~MultiResolutionWidget();

          Ui::MultiResolutionWidgetForm* getForm() const;

        public:

          bool createMultiResolution();

          void setLayerList(std::list<te::map::AbstractLayerPtr>& layerList);

        protected:

          bool fromLayer();

          bool fromFile();

          bool fromFolder();

          bool createMultiRes(te::rst::Raster* raster);

          void fillInterpolatorTypes();

        protected slots:

          void onFileToolButtonClicked();

          void onFolderToolButtonClicked();

        private:

          std::auto_ptr<Ui::MultiResolutionWidgetForm> m_ui;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RASTER_INTERNAL_MULTIRESOLUTIONWIDGET_H

