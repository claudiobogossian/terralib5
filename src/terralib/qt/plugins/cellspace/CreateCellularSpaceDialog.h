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
  \file terralib/qt/widgets/connector/ado/ADOConnectorDialog.h

  \brief ....
*/

#ifndef __TERRALIB_QT_PLUGINS_CELLSPACE_INTERNAL_CREATECELLULARSPACEDIALOG_H
#define __TERRALIB_QT_PLUGINS_CELLSPACE_INTERNAL_CREATECELLULARSPACEDIALOG_H

// TerraLib
#include "../../../common/UnitOfMeasure.h"
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../maptools/AbstractLayer.h"

// STL
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class CreateCellularSpaceDialogForm; }

namespace te
{
  namespace gm { class Envelope; }

  namespace qt
  {
    namespace plugins
    {
      namespace cellspace
      {
        /*!
          \class CreateCellularSpaceDialog

          \brief ....
        */
        class CreateCellularSpaceDialog : public QDialog
        {
          Q_OBJECT

          public:

            CreateCellularSpaceDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

            ~CreateCellularSpaceDialog();

            void setLayers(std::list<te::map::AbstractLayerPtr> layers);

            te::map::AbstractLayerPtr getLayer();

          protected slots:

            void onLayersComboBoxChanged(int index);

            void onUnitComboBoxChanged(int index);

            void onResXLineEditTextChanged(const QString & text);

            void onResYLineEditTextChanged(const QString & text);

            void onEnvelopeChanged(const QString & text);

            void onTargetFileToolButtonClicked();

            void onTargetDatasourceToolButtonClicked();

            void onCreatePushButtonClicked();

            void onVectorToolButtonToggled(bool isToggled);

            void onPointsToolButtonToggled(bool isToggled);

            void onRasterToolButtonToggled(bool isToggled);

            void onSrsToolButtonClicked();

          private:

            te::common::UnitOfMeasurePtr getCurrentUnit();

            void setCurrentUnit(te::common::UnitOfMeasurePtr unit);

            void initUnitsOfMeasure();

            void showEnvelope(const te::gm::Envelope env);

            void showSRS();

            bool isBasicInfoSet();

            void updateValues();

            bool checkList(std::string& errors);

            te::gm::Envelope getOutputEnvelope();

            te::da::DataSourceInfoPtr getDataSourceInfo();

            te::map::AbstractLayerPtr getReferenceLayer();

            te::gm::Envelope getEnvelope();

            void clearEnvelope();

            void clearResolution();

            bool isNone();

            double getResX();

            double getResY();

          private:

            std::auto_ptr<Ui::CreateCellularSpaceDialogForm> m_ui;
            bool m_isFile;
            te::da::DataSourceInfoPtr m_outDataSourceInfo;
            std::string m_outputDataSetName;
            int m_currentSRID;
        }; 
      }   // end namespace cellspace
    }     // end namespace plugins
  }       // end namespace qt
}         // end namespace te

#endif  // __TERRALIB_QT_PLUGINS_CELLSPACE_INTERNAL_CREATECELLULARSPACEDIALOG_H

