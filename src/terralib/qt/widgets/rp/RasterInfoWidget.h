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
  \file terralib/qt/widgets/rp/RasterInfoWidget.h

  \brief This file has the RasterInfoWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_RASTERINFOWIDGET_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_RASTERINFOWIDGET_H

// TerraLib
#include "../Config.h"

// STL
#include <memory>
#include <string>
#include <map>

// Qt
#include <QWidget>

namespace Ui { class RasterInfoWidgetForm; }

namespace te
{
  namespace da { class DataSource; }

  namespace qt
  {
    namespace widgets
    {
      class ParameterTableWidget;

      /*!
        \class RasterInfoWidget

        \brief This class is used to define a map of information necessary to create a raster.

        \sa RasterFactory
      */
      class TEQTWIDGETSEXPORT RasterInfoWidget : public QWidget
      {
        Q_OBJECT

        public:

          /*!
            \brief Default constructor.

            \param outputMode Allows select input rasters (outputMode=false - rasters that already exists) or output raster info - outputMode=true).
          */
          RasterInfoWidget(const bool outputMode, QWidget* parent, Qt::WindowFlags f);

          ~RasterInfoWidget();

          Ui::RasterInfoWidgetForm* getForm() const;

          std::string getType() const;

          std::map<std::string, std::string> getInfo() const;

          std::map<std::string, std::string> getInfo(int count) const;

          std::auto_ptr<te::da::DataSource> getDataSource() const;

          std::string getName() const;

          std::string getShortName() const;

          std::string getExtension() const;
          
          std::string getPath() const;

          bool overight() const;

          bool fileExists() const;
          
        protected slots:

          void onOpenFileDlgToolButtonClicked();          

        protected:

          void fillExtensions();

        private:

         bool m_outputMode; //!< Allows select input rasters (outputMode=false - rasters that already exists) or output raster info - outputMode=true).
         std::auto_ptr<Ui::RasterInfoWidgetForm> m_ui;
         std::auto_ptr<te::qt::widgets::ParameterTableWidget> m_table;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_RASTERINFOWIDGET_H

