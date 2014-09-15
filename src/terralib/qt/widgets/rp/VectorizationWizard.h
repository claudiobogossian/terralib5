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


#ifndef TE_QT_WIDGETS_VECTORIZATIONWIZARD_H
#define TE_QT_WIDGETS_VECTORIZATIONWIZARD_H

//TerraLib
#include "../Config.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../maptools/AbstractLayer.h"

//Qt
#include <QWizard>

//STL
#include <memory>

namespace te 
{
  //forward declarations
  namespace da  { class DataSetType; }
  namespace mem { class DataSet; }

  namespace qt 
  {
    namespace widgets 
    {
      //Forward declarations
      class VectorizationWizardPage;
      class LayerSearchWizardPage;

      class TEQTWIDGETSEXPORT VectorizationWizard : public QWizard
      {
        public:

          VectorizationWizard(QWidget* parent=0);

          ~VectorizationWizard();

          bool validateCurrentPage();

          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

          te::map::AbstractLayerPtr getOutputLayer();

        protected:

          void addPages();

          bool execute();

          /*! Function used to create the output dataset type */
          std::auto_ptr<te::da::DataSetType> createDataSetType(std::string dataSetName, int srid);

          /*! Function used to create the output data */
          std::auto_ptr<te::mem::DataSet> createDataSet(te::da::DataSetType* dsType, std::vector<te::gm::Geometry*>& geoms);

          /*! Function used to save the output dataset */
          void saveDataSet(te::mem::DataSet* dataSet, te::da::DataSetType* dsType, te::da::DataSourcePtr ds, std::string dataSetName);

        private:

          std::auto_ptr<te::qt::widgets::VectorizationWizardPage> m_vectorPage;
          std::auto_ptr<te::qt::widgets::LayerSearchWizardPage> m_layerSearchPage;

          te::map::AbstractLayerPtr m_outputLayer;
      };
      
    } // namespace widgets
  } // namespace qt
} // namespace te

#endif // TE_QT_WIDGETS_VECTORIZATIONWIZARD_H
