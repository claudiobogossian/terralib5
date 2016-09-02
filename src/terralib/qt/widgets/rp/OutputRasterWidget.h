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
  \file terralib/qt/widgets/rp/OutputRasterWidget.h

  \brief This file has the OutputRasterWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_OUTPUTRASTERWIDGET_H
#define __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_OUTPUTRASTERWIDGET_H

// TerraLib
#ifndef Q_MOC_RUN
#include "../../../maptools/AbstractLayer.h"
#endif
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWidget>

namespace Ui { class OutputRasterWidgetForm; }

namespace te
{
  namespace rst { class Raster; }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class OutputRasterWidget

        \brief This class is used to set output layer.
      */
      class TEQTWIDGETSEXPORT OutputRasterWidget : public QWidget
      {
        Q_OBJECT

        public:

          OutputRasterWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~OutputRasterWidget();

          Ui::OutputRasterWidgetForm* getForm() const;

          std::string getOutputRaster();

          bool fileExists() const;

          std::string getType() const;

          std::map<std::string, std::string> getInfo() const;

        protected slots:

          void onTargetFileToolButtonClicked();

          void onTargetDatasourceToolButtonClicked();

        private:

         std::auto_ptr<Ui::OutputRasterWidgetForm> m_ui;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_OUTPUTRASTERWIDGET_H

