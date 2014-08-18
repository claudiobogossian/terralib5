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
  \file terralib/vp/qt/BasicGeometricOpWizardPage.h

  \brief This file defines a class for a Basic Geometric Operation Wizard Page.
*/

#ifndef __TERRALIB_VP_INTERNAL_GEOMETRICOPOUTPUTWIZARDPAGE_H
#define __TERRALIB_VP_INTERNAL_GEOMETRICOPOUTPUTWIZARDPAGE_H

// TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../maptools/AbstractLayer.h"
#include "../Config.h"
#include "../Enums.h"

// STL
#include <memory>

// Qt
#include <QWizardPage>

// Forward declaration
namespace Ui { class GeometricOpOutputWizardPageForm; }

namespace te
{
  namespace vp
  {
    /*!
      \class GeometricOpOutputWizardPage

      \brief This class is GUI used to define the basic geometric operator parameters for the VP operation.
    */
    class TEVPEXPORT GeometricOpOutputWizardPage : public QWizardPage
    {
      Q_OBJECT

      public:

        GeometricOpOutputWizardPage(QWidget* parent = 0);

        ~GeometricOpOutputWizardPage();

      public:

        bool hasConvexHull();

        bool hasCentroid();

        bool hasMBR();

        bool hasArea();

        bool hasLine();

        bool hasPerimeter();

        std::string getAttribute();

        void setAttributes(std::vector<std::string> attributes);

        te::vp::GeometricOpObjStrategy getObjectStrategy();

        std::string getOutDsName();

        bool hasOutputLayer();

        bool getToFile();

        te::da::DataSourceInfoPtr getDsInfoPtr();

        std::string getPath();

      public slots:

        void onAllObjectsToggled();

        void onSimpleOperationToggled();

        void onAttributeOperationToggled();

        void onAttributeComboBoxChanged(int index);

        void onTargetDatasourceToolButtonPressed();

        void onTargetFileToolButtonPressed();

      protected:

       // void loadAttributes();

      private:

        std::auto_ptr<Ui::GeometricOpOutputWizardPageForm> m_ui;

        te::da::DataSourceInfoPtr m_outputDatasource;                 //!< DataSource information.
        std::string m_dsName;
        bool m_toFile;
        std::string m_attribute;
        std::string m_path;

    };

  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_GEOMETRICOPOUTPUTWIZARDPAGE_H
