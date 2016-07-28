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


#ifndef TE_QT_WIDGETS_RASTERSLICINGWIZARDPAGE_H
#define TE_QT_WIDGETS_RASTERSLICINGWIZARDPAGE_H

//Terralib
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

//Qt
#include <QWizardPage>

//STL
#include <memory>

//Forward declarations
namespace Ui
{
  class RasterSlicingWizardPageForm;
}

namespace te 
{
  namespace se
  {
    class ColorMap;
  }

  namespace qt 
  {
    namespace widgets 
    {
      class ChartStyle;
      class ChartDisplay;
      class ColorMapWidget;
      class Histogram;
      class HistogramChart;

      class TEQTWIDGETSEXPORT RasterSlicingWizardPage : public QWizardPage
      {
          Q_OBJECT

        public:
          
          RasterSlicingWizardPage(QWidget* parent=0);

          ~RasterSlicingWizardPage();

          te::se::ColorMap* getColorMap();

          std::string getCurrentBand();

          const te::gm::Envelope& getExtent();

          int getSRID();

          bool trimRaster();

          void setRaster(te::rst::Raster* raster);

          void setExtent(const te::gm::Envelope& extent);

          void setSRID(int srid = 0);
          
          void getParameters( unsigned int& inputRasterBand, bool& createPaletteRaster,
            unsigned int& slicesNumber, bool& eqHistogram );

          bool isComplete() const;
          
        protected :

          std::auto_ptr<Ui::RasterSlicingWizardPageForm>  m_ui;
          te::qt::widgets::ChartStyle*                    m_chartStyle;
          te::qt::widgets::ChartDisplay*                  m_chartDisplay;
          te::qt::widgets::ColorMapWidget*                m_colorMapWidget;
          te::qt::widgets::Histogram*                     m_histogram;
          te::qt::widgets::HistogramChart*                m_histogramChart;
          std::auto_ptr<te::rst::Raster>                  m_raster;

        protected slots:

          void onApplyPushButtonClicked();
          void onHistBandComboBoxIndexCHanged(int index);
          void onSliceBandComboBoxIndexCHanged(int index);

      private:

        te::gm::Envelope m_extent;
        int m_srid;
      };      
    } // namespace widgets
  } // namespace qt
} // namespace te

#endif // TE_QT_WIDGETS_RASTERSLICINGWIZARDPAGE_H
