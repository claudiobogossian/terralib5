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


#include "VectorizationWizard.h"

#include "../help/HelpPushButton.h"
#include "../layer/search/LayerSearchWidget.h"

#include "LayerSearchWizardPage.h"
#include "RasterInfoWizardPage.h"
#include "VectorizationWizardPage.h"

namespace te {
  namespace qt {
    namespace widgets {
      
      VectorizationWizard::VectorizationWizard(QWidget* parent) :
        QWizard(parent)
      {
        this->setWizardStyle(QWizard::ModernStyle);
        this->setWindowTitle(tr("Vectorization"));

        this->setOption(QWizard::HaveHelpButton, true);
        this->setOption(QWizard::HelpButtonOnRight, false);

        te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

        this->setButton(QWizard::HelpButton, helpButton);

        helpButton->setPageReference("plugins/rp/rp_vectorization.html");

        addPages();
      }

      VectorizationWizard::~VectorizationWizard()
      {
      }

      bool VectorizationWizard::validateCurrentPage()
      {
        return true;
      }

      void VectorizationWizard::setList(std::list<te::map::AbstractLayerPtr> &layerList)
      {
        m_layerSearchPage->getSearchWidget()->setList(layerList);
        m_layerSearchPage->getSearchWidget()->filterOnlyByRaster();
      }

      te::map::AbstractLayerPtr VectorizationWizard::getOutputLayer()
      {
        return m_outputLayer;
      }

      void VectorizationWizard::addPages()
      {
        m_layerSearchPage.reset(new LayerSearchWizardPage(this));
        addPage(m_layerSearchPage.get());

        m_vectorPage.reset(new VectorizationWizardPage(this));
        addPage(m_vectorPage.get());

        m_rasterInfoPage.reset(new RasterInfoWizardPage(this));
        addPage(m_rasterInfoPage.get());
      }

      bool VectorizationWizard::execute()
      {
        return true;
      }
      
    } // namespace widgets
  } // namespace qt
} // namespace te
