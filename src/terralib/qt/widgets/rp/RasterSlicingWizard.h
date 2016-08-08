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


#ifndef TE_QT_WIDGETS_RASTERSLICINGWIZARD_H
#define TE_QT_WIDGETS_RASTERSLICINGWIZARD_H

//TerraLib
#include "../Config.h"
#include "../../../maptools/AbstractLayer.h"

//Qt
#include <QWizard>

//STL
#include <memory>

namespace te 
{
  namespace qt 
  {
    namespace widgets 
    {
      //Forward declarations
      class RasterSlicingWizardPage;
      class LayerSearchWizardPage;
      class RasterInfoWizardPage;

      class TEQTWIDGETSEXPORT RasterSlicingWizard : public QWizard
      {
        public:

          /*! \brief Constructor. */
          RasterSlicingWizard(QWidget* parent=0);

          /*! \brief Destructor. */
          ~RasterSlicingWizard();

          /*!
          \brief Overridden function that checks if the current page's configuration is done in order to proceed to the next page.

          \return A boolean that indicates if the current page's configuration is finished.
          */
          bool validateCurrentPage();

          /*!
          \brief This function sets the list of layers from which the raster that will be sliced will come from.

          \param layerList The list of AbstractLayerPtrs that will be used to get a raster from.
          */
          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

          /*!
          \brief This function returns the layer that has been created by the wizard with the sliced raster.

          \retunrn A new AbstractLayerPtr containing the sliced raster.
          */
          te::map::AbstractLayerPtr getOutputLayer();

          /*!
          \brief This function sets the envelope that will be used to trim tha raster if the user requests it.

          \param extent A reference to the envelope that can be used to trim tha raster.
          */
          void setExtent(const te::gm::Envelope& extent);

          /*!
          \brief This function sets the srid of the display when the wizard was initiated.

          \param srid The current srid of the display.
          */
          void setSRID(int srid = 0);

        protected:

          void addPages();

          bool execute();

        private:

          //No copy allowed
          RasterSlicingWizard(const RasterSlicingWizard& rhs);
          RasterSlicingWizard& operator=(const RasterSlicingWizard& rhs);


          std::auto_ptr<te::qt::widgets::RasterSlicingWizardPage>  m_wizardPage;       //!< The wizard page used to define the slicing parameters
          std::auto_ptr<te::qt::widgets::LayerSearchWizardPage>    m_layerSearchPage;  //!< The wizard page used to select an input layer
          std::auto_ptr<te::qt::widgets::RasterInfoWizardPage>     m_rasterInfoPage;   //!< The wizard page used to define the output layer parameters

          te::map::AbstractLayerPtr m_outputLayer;
      };      
    } // namespace widgets
  } // namespace qt
} // namespace te

#endif // TE_QT_WIDGETS_RASTERSLICINGWIZARD_H
