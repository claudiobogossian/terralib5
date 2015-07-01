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
  \file terralib/qt/widgets/rp/OutputRasterParametersWidget.h

  \brief This file has the OutputRasterParametersWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_OUTPUTRASTERPARAMETERSWIDGET_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_OUTPUTRASTERPARAMETERSWIDGET_H

// TerraLib
#include "../Config.h"

// STL
#include <memory>
#include <string>
#include <map>

// Qt
#include <QWidget>

namespace Ui { class OutputRasterParametersWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class RasterInfoDialog;

      /*!
        \class OutputRasterParametersWidget

        \brief This class represents a component used to quickly define a raster parameters

        \sa RasterInfoWidget
      */
      class TEQTWIDGETSEXPORT OutputRasterParametersWidget : public QWidget
      {
        Q_OBJECT

        public:

          OutputRasterParametersWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~OutputRasterParametersWidget();

          Ui::OutputRasterParametersWidgetForm* getForm() const;

        public:

          std::string getType() const;

          std::map<std::string, std::string> getInfo() const;

          std::string getName() const;

          bool overight() const;

        public slots:

          void onSourceToolButtonClicked();

          void onFileToolButtonClicked();

        private:

         std::auto_ptr<Ui::OutputRasterParametersWidgetForm> m_ui;
         std::auto_ptr<RasterInfoDialog> m_rasterInfoDlg;

         bool m_fileGDAL;
         bool m_dataSource;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_OUTPUTRASTERPARAMETERSWIDGET_H
