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
  \file terralib/qt/widgets/exchanger/DirectExchangerDialog.h

  \brief A direct exchanger dialog for ADO, POSTGIS and SHP data sources
*/

#ifndef __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_DIRECTEXCHANGERDIALOG_H
#define __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_DIRECTEXCHANGERDIALOG_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class DirectExchangerDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
// Forward declarations

      class TEQTWIDGETSEXPORT DirectExchangerDialog : public QDialog
      {
        Q_OBJECT

        public:

          DirectExchangerDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~DirectExchangerDialog();

          /*!
            \brief Set the layer that can be used

            \param layers   List of AbstractLayerPtr
          */
          void setLayers(std::list<te::map::AbstractLayerPtr> layers);

          /*!
            \brief Function used to set the last data source used

            \param dataSource   Last data source name used
          */
          void setLastDataSource(std::string dataSource);

        
        protected:

          /*!
            \brief Set the list of data sources that can be used
          */
          void setDataSources();

          bool exchangeToFile();

          bool exchangeToDatabase();

        protected slots:

          void onDataSourceTypeActivated(int index);

          void onInputLayerActivated(QString value);

          void onDirToolButtonClicked();

          void onDataSoruceToolButtonClicked();

          void onOkPushButtonClicked();

          void setOutputDataSources();

        private:

          std::auto_ptr<Ui::DirectExchangerDialogForm> m_ui;

          std::list<te::map::AbstractLayerPtr> m_layers;

          std::string m_outputDataSourceType;

          std::string m_lastDsType;

          bool m_exchangeToFile;
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_DIRECTEXCHANGERDIALOG_H