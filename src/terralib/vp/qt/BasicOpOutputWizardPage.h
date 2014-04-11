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
  \file terralib/vp/qt/BasicGeopraphicOpWizardPage.h

  \brief This file defines a class for a Basic Geographic Operation Wizard Page.
*/

#ifndef __TERRALIB_VP_INTERNAL_BASICOPOUTPUTWIZARDPAGE_H
#define __TERRALIB_VP_INTERNAL_BASICOPOUTPUTWIZARDPAGE_H

// TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../maptools/AbstractLayer.h"
#include "../Config.h"
#include "../Enums.h"

// STL
#include <memory>

// Qt
#include <QtGui/QWizardPage>

// Forward declaration
namespace Ui { class BasicOpOutputWizardPageForm; }

namespace te
{
  namespace vp
  {
    /*!
      \class BasicOpOutputWizardPage

      \brief This class is GUI used to define the basic geographic operator parameters for the VP operation.
    */
    class TEVPEXPORT BasicOpOutputWizardPage : public QWizardPage
    {
      Q_OBJECT

      public:

        BasicOpOutputWizardPage(QWidget* parent = 0);

        ~BasicOpOutputWizardPage();

      public:

        bool hasConvexHull();

        bool hasCentroid();

        bool hasMBR();

        bool hasArea();

        bool hasLine();

        bool hasPerimeter();

        std::string getAttribute();

        void setAttributes(std::vector<std::string> attributes);

        std::string getOutDsName();

        bool getToFile();

        te::da::DataSourceInfoPtr getDsInfoPtr();

        std::string getPath();

      public slots:

        void onSimpleOperationToggled();

        void onAttributeOperationToggled();

        void onAttributeComboBoxChanged(int index);

        void onTargetDatasourceToolButtonPressed();

        void onTargetFileToolButtonPressed();

      protected:

       // void loadAttributes();

      private:

        std::auto_ptr<Ui::BasicOpOutputWizardPageForm> m_ui;

        te::da::DataSourceInfoPtr m_outputDatasource;                 //!< DataSource information.
        std::string m_dsName;
        bool m_toFile;
        std::string m_attribute;

    };

  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_BASICGEOGRAPHICOPWIZARDPAGE_H
