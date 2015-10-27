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


#ifndef TE_QT_WIDGETS_RASTERIZATIONWIZARDPAGE_H
#define TE_QT_WIDGETS_RASTERIZATIONWIZARDPAGE_H

//Terralib
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

//Qt
// QT
#include <QTableWidgetItem>
#include <QWizardPage>

//STL
#include <memory>

//Forward declarations
namespace Ui
{
  class GroupingWidgetForm;
  class RasterizationWizardPageForm;
}

namespace te 
{
  namespace color { class ColorBar; }

  namespace map
  {
    class Grouping;
    class GroupingItem;
  }

  namespace rst { class Raster; }

  namespace qt 
  {
    namespace widgets 
    {
      
      // Forward declarations
      class ColorCatalogWidget;

      class TEQTWIDGETSEXPORT RasterizationWizardPage : public QWizardPage
      {
          Q_OBJECT

        public:

          RasterizationWizardPage(QWidget* parent=0);

          ~RasterizationWizardPage();

          void setLayer(te::map::AbstractLayerPtr layer);

          double getResX();

          double getResY();

          te::gm::Envelope getEnvelope();
          
          int getSrid();

          bool outputDataSourceToFile();

          te::da::DataSourceInfoPtr getDataSourceInfo();

          std::string getRepositoryName();

          std::string getAttributeName();

          std::map<std::string, std::vector<int> > getInformations();

          void validate();

          std::vector<te::map::GroupingItem*> getGroupingItems();

          QTableWidget* getTableWidget();

        private:

          /*! \brief Updates the widget form based on internal fill element. */
          void updateUi(bool loadColorBar = false);

          std::vector<std::string> getDistinctClasses(const std::string& attrName);

          void getDataAsString(std::vector<std::string>& vec, const std::string& attrName, int& nullValues);

          void getLinkedDataAsString(std::vector<std::string>& vec, const std::string& attrName, int& nullValues);

          void buildSymbolizer(std::string meanTitle = "");

          void createStringNullGroupingItem(int count);

          int getGeometryType();

          void listAttributes();

        protected slots:

          void onApplyPushButtonClicked();

          void onSelectAllPushButtonClicked();

          void onUnselectAllPushButtonClicked();

          void onAttrComboBoxActivated(int idx);

          void onColorBarChanged();

          void onTableWidgetItemChanged(QTableWidgetItem* item);

          void onTableWidgetItemDoubleClicked(QTableWidgetItem* item);

          void onLoadPushButtonClicked();

          void onResXLineEditEditingFinished();

          void onResYLineEditEditingFinished();

          void onTargetFileToolButtonPressed();

        signals:

          void applyPushButtonClicked();

        private:

          std::auto_ptr<Ui::RasterizationWizardPageForm> m_ui;

          te::map::AbstractLayerPtr m_layer;                        //!< TerraLib layer auto ptr

          te::color::ColorBar* m_cb;                                //!< Terralib color bar objetc
          te::qt::widgets::ColorCatalogWidget* m_colorBar;         //!< Widget used to pick a color.

          std::vector<te::map::GroupingItem*> m_legend;             //!< Grouping items

          te::da::DataSourceInfoPtr m_outputDatasource;                                     //!< DataSource information.

          bool m_toFile;

          bool m_manual;
      };
      
    } // namespace widgets
  } // namespace qt
} // namespace te

#endif // TE_QT_WIDGETS_RASTERIZATIONWIZARDPAGE_H
