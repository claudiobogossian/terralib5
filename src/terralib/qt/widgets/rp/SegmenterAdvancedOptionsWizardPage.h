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
  \file terralib/qt/widgets/rp/SegmenterAdvancedOptionsWizardPage.h

  \brief This file defines a class for a Segmenter Advanced Options Wizard page.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_SEGMENTERADVANCEDOPTIONSWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_SEGMENTERADVANCEDOPTIONSWIZARDPAGE_H

// TerraLib
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWizardPage>

// Forward declaration
namespace Ui { class SegmenterAdvancedOptionsWizardPageForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class SegmenterAdvancedOptionsWizardPage

        \brief This class is GUI used to define the segmenter advanced options parameters for the RP constast operation.
      */
      class TEQTWIDGETSEXPORT SegmenterAdvancedOptionsWizardPage : public QWizardPage
      {
        public:

          SegmenterAdvancedOptionsWizardPage(QWidget* parent = 0);

          ~SegmenterAdvancedOptionsWizardPage();

        public:

          Ui::SegmenterAdvancedOptionsWizardPageForm* getForm() const;

        private:

          std::auto_ptr<Ui::SegmenterAdvancedOptionsWizardPageForm> m_ui;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_SEGMENTERADVANCEDOPTIONSWIZARDPAGE_H
