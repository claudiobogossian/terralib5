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
  \file src/terraMobilePlugin/qt/GeoPackageBuilderWizard.h

  \brief This interface is used to get the input parameters for GeoPackage Builder wizard operation.
*/

#ifndef __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_GEOPACKAGEBUILDERWIZARD_H
#define __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_GEOPACKAGEBUILDERWIZARD_H

// TerraLib
#include "../../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWizard>

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace terramobile
      {
        class BuilderInputLayersWizardPage;
        class BuilderGatheringLayersWizardPage;
        class BuilderFormsWizardPage;
        class BuilderOutputWizardPage;

        /*!
          \class GeoPackageBuilderWizard

          \brief This interface is used to get the input parameters for GeoPackage Builder wizard operation.
        */
        class GeoPackageBuilderWizard : public QWizard
        {
          Q_OBJECT

          public:

            GeoPackageBuilderWizard(QWidget* parent = 0, Qt::WindowFlags f = 0);

            ~GeoPackageBuilderWizard();

          public:

            void exportToGPKG(te::map::AbstractLayerPtr layer, te::da::DataSource* dsGPKG, std::string outFileName);

           void setLayerList(std::list<te::map::AbstractLayerPtr> list);

          protected:

            virtual bool validateCurrentPage();

            void addPages();

            bool execute();

            std::list<te::map::AbstractLayerPtr> getGatheringLayers();

            std::list<te::map::AbstractLayerPtr> getInputLayers();

          private:

            std::list<te::map::AbstractLayerPtr> m_list;

            std::auto_ptr<te::qt::plugins::terramobile::BuilderInputLayersWizardPage> m_inputLayersPage;
            std::auto_ptr<te::qt::plugins::terramobile::BuilderGatheringLayersWizardPage> m_gatheringLayersPage;
            std::auto_ptr<te::qt::plugins::terramobile::BuilderFormsWizardPage> m_formsPage;
            std::auto_ptr<te::qt::plugins::terramobile::BuilderOutputWizardPage> m_outputPage;
        }; 
      }   // end namespace thirdParty
    }     // end namespace plugins
  }       // end namespace qt
}         // end namespace te

#endif  // __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_GEOPACKAGEBUILDERWIZARD_H

