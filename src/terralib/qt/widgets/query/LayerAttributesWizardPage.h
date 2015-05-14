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

/*!
  \file terralib/qt/widgets/query/LayerAttributesWizardPage.h

  \brief This file has the LayerAttributesWizardPage class.
*/

#ifndef __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_LAYERATTRIBUTESWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_LAYERATTRIBUTESWIZARDPAGE_H

// TerraLib
#include "../Config.h"

// STL
#include <string>
#include <memory>

// Qt
#include <QWizardPage>

namespace Ui { class LayerAttributesWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class LayerAttributesWizardPage

        \brief This class is used to define the layer attributes information.
      */
      class TEQTWIDGETSEXPORT LayerAttributesWizardPage : public QWizardPage
      {
        Q_OBJECT

        public:

          LayerAttributesWizardPage(QWidget* parent = 0);

          ~LayerAttributesWizardPage();

          bool isComplete() const;


        public:

          std::string getLayerName();

        public slots:

          void layerNameChanged(QString);


        private:

         std::auto_ptr<Ui::LayerAttributesWidgetForm> m_ui;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_LAYERATTRIBUTEWIZARDPAGE_H

