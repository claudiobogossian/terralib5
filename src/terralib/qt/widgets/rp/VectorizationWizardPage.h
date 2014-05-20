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


#ifndef TE_QT_WIDGETS_VECTORIZATIONWIZARDPAGE_H
#define TE_QT_WIDGETS_VECTORIZATIONWIZARDPAGE_H

//Terralib
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

//Qt
#include <QtGui/QWizardPage>

//STL
#include <memory>

//Forward declarations
namespace Ui
{
  class VectorizationWizardPageForm;
}

namespace te 
{
  namespace rst { class Raster; }

  namespace qt 
  {
    namespace widgets 
    {
      
      class TEQTWIDGETSEXPORT VectorizationWizardPage : public QWizardPage
      {
          Q_OBJECT

        public:
          VectorizationWizardPage(QWidget* parent=0);

          ~VectorizationWizardPage();

          bool isComplete() const;

        public:

          /*!
            \brief This method is used to set the selected layer for vectorization operation
            
            \param layer The layer ptr 

            \note This layer MUST HAVE a valid raster object.
          */
          void setLayer(te::map::AbstractLayerPtr layer);

          std::auto_ptr<te::rst::Raster> getRaster();

          unsigned int getBand();

          bool hasMaxGeom(unsigned int& maxGeom);

          bool outputDataSourceToFile();

          te::da::DataSourceInfoPtr getDataSourceInfo();

          std::string getLayerName();

          std::string getRepositoryName();

        protected slots:

          void onTargetDatasourceToolButtonPressed();

          void onTargetFileToolButtonPressed();

        private:

          std::auto_ptr<Ui::VectorizationWizardPageForm> m_ui;

          te::map::AbstractLayerPtr m_layer;

          te::da::DataSourceInfoPtr m_outputDatasource;                                     //!< DataSource information.

          bool m_toFile;
      };
      
    } // namespace widgets
  } // namespace qt
} // namespace te

#endif // TE_QT_WIDGETS_VECTORIZATIONWIZARDPAGE_H
