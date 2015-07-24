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
            
            void onEnvelopeChanged(const QString & text);
            
            void onSrsToolButtonClicked();
            
            
            void onUnitComboBoxChanged(int index);
            
            void onResXLineEditEditingFinished();
            
            void onResYLineEditEditingFinished();
            
            void onColsLineEditEditingFinished();
            
            void onRowsLineEditEditingFinished();
            
            
            void onTargetDatasourceToolButtonClicked();
            
            void onTargetFileToolButtonClicked();
            
          void onCreatePushButtonClicked();


          private:
          
          
            void initUnitsOfMeasure();
            
            
            void setResolutionUnit(te::common::UnitOfMeasurePtr unit);
            
            te::common::UnitOfMeasurePtr getResolutionUnit();
            
            
            te::da::DataSourceInfoPtr getDataSourceInfo();
            
            te::map::AbstractLayerPtr getReferenceLayer();
            
            te::gm::Envelope getEnvelope();
            
            void showEnvelope(const te::gm::Envelope& env, int precision);
            
            void clearEnvelope();
   
            double getResX();
            
            double getResY();
            
            bool unitConvertion(double& res,
                              te::common::UnitOfMeasurePtr from,
                              te::common::UnitOfMeasurePtr to);

            void clearResolution();
          

            void showSRS();

            bool checkList(std::string& errors);
          
            bool convertAngleToPlanar(double& val, te::common::UnitOfMeasurePtr planar);
          
            bool convertPlanarToAngle(double& val, te::common::UnitOfMeasurePtr planar);

          private:
          
            int                        m_bbSRID;
            bool                       m_isFile;
            te::da::DataSourceInfoPtr  m_outDataSourceInfo;
            std::string                m_outputDataSetName;

            std::auto_ptr<Ui::CreateCellularSpaceDialogForm> m_ui;

        }; 
      }   // end namespace cellspace
    }     // end namespace plugins
  }       // end namespace qt
}         // end namespace te

#endif  // __TERRALIB_QT_PLUGINS_CELLSPACE_INTERNAL_CREATECELLULARSPACEDIALOG_H

