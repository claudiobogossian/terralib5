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
  \file terralib/qt/plugins/terralib5/TL4ConverterWizard.h

  \brief ????
*/

#ifndef __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4CONVERTERWIZARD_H
#define __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4CONVERTERWIZARD_H

// TerraLib
#include "Config.h"

// STL
#include <memory>

// Qt
#include <QtGui/QWizard>

// Forward declaration
namespace Ui { class TL4ConverterWizardForm; }

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace terralib4
      {
        class TL4ConnectorWizardPage;
        class TL4LayerSelectionWizardPage;

        class TEQTPLUGINTERRALIB4EXPORT TL4ConverterWizard : public QWizard
        {
          Q_OBJECT

          public:

            TL4ConverterWizard(QWidget* parent = 0, Qt::WindowFlags f = 0);

            ~TL4ConverterWizard();

            int nextId() const;

          protected slots:

            void back();

            void next();

            void commit();

            void help();

          private:

            std::auto_ptr<Ui::TL4ConverterWizardForm> m_ui;
            std::auto_ptr<TL4ConnectorWizardPage> m_connectorPage;
            std::auto_ptr<TL4LayerSelectionWizardPage> m_layerSelectionPage;
        };
      } // end namespace terralib4
    } // end namespace plugins
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4CONVERTERWIZARD_H
