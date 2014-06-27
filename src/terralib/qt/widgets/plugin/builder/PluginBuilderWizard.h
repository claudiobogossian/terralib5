/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/plugin/builder/PluginBuilderWizard.h

  \brief A Qt dialog that allows users to create new plugins based on TerraLib framework.
*/

#ifndef __TERRALIB_QT_WIDGETS_PLUGIN_BUILDER_INTERNAL_PLUGINBUILDERWIZARD_H
#define __TERRALIB_QT_WIDGETS_PLUGIN_BUILDER_INTERNAL_PLUGINBUILDERWIZARD_H

// TerraLib
#include "../../Config.h"

// STL
#include <memory>

// Qt
#include <QWizard>

// Forward declaration
namespace Ui { class PluginBuilderWizardForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class DoubleListWidget;
      class ParameterTableWidget;

      class TEQTWIDGETSEXPORT PluginBuilderWizard : public QWizard
      {
        Q_OBJECT

        public:

          PluginBuilderWizard(QWidget* parent);

          ~PluginBuilderWizard();

          virtual bool validateCurrentPage();

        public slots:

          void onTeIncludeDirButtonClicked();
          void onTeCmakeDirButtonClicked();
          void onPluginSrcDirButtonClicked();
          void onPluginBuildDirButtonClicked();

        protected:

          bool pluginInfoPageCheck();
          bool providerPageCheck();
          bool dirPageCheck();
          void buildPlugin();

        private:

          std::auto_ptr<Ui::PluginBuilderWizardForm> m_ui;
          std::auto_ptr<DoubleListWidget> m_pluginDependencies;
          std::auto_ptr<DoubleListWidget> m_categoryDependencies;
          std::auto_ptr<DoubleListWidget> m_moduleDependencies;
          std::auto_ptr<ParameterTableWidget> m_pluginResources;
          std::auto_ptr<ParameterTableWidget> m_pluginParameters;

          QString m_curDir;   //!< Attribute used to keep the last dir selected
      };


    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_PLUGIN_BUILDER_INTERNAL_PLUGINBUILDERWIZARD_H
