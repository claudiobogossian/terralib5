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
  \file src/terraMobilePlugin/qt/BuilderFormsWizardPage.h

  \brief This interface is used to configure the form json file for GeoPackage Builder wizard operation.
*/

#ifndef __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_BUILDERFORMSWIZARDPAGE_H
#define __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_BUILDERFORMSWIZARDPAGE_H

// TerraLib
#include "../../../../maptools/AbstractLayer.h"
#include "../core/form/AbstractFormItem.h"
#include "../core/form/Form.h"
#include "../core/form/Section.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QTreeWidgetItem>
#include <QWizardPage>

// Forward declaration
namespace Ui { class BuilderFormsWizardPageForm; }

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace terramobile
      {

        class Section;
        /*!
          \class BuilderFormsWizardPage

          \brief This interface is used to configure the form json file for GeoPackage Builder wizard operation.
        */
        class BuilderFormsWizardPage : public QWizardPage
        {
          Q_OBJECT

          public:

            BuilderFormsWizardPage(QWidget* parent = 0);

            ~BuilderFormsWizardPage();

          public:

            bool isComplete() const;

            void setLayerList(std::list<te::map::AbstractLayerPtr> list);

            std::map<std::string, Section*>& getSections();

          public slots:

            void onTreeItemClicked(QTreeWidgetItem* item, int column);

            void onStringComboAddToolButtonPressed();

            void onSaveFormItemToolButton();

          private:

            std::auto_ptr<Ui::BuilderFormsWizardPageForm> m_ui;

            std::list<te::map::AbstractLayerPtr> m_list;

            std::map<std::string, Section*> m_sectionsMap;
            te::qt::plugins::terramobile::Section* m_section;

            te::qt::plugins::terramobile::Form* m_curForm;
            te::qt::plugins::terramobile::AbstractFormItem* m_curFormItem;

            int m_itemTreeType;
        }; 
      }   // end namespace thirdParty
    }     // end namespace plugins
  }       // end namespace qt
}         // end namespace te

#endif  // __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_BUILDERFORMSWIZARDPAGE_H

