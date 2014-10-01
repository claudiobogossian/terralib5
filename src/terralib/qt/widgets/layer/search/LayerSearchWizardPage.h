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
  \file terralib/qt/widgets/layer/search/LayerSearchWizardPage.h

  \brief This file defines the LayerSearchWizardPage class.
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_SEARCH_INTERNAL_LAYERSEARCHWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_LAYER_SEARCH_INTERNAL_LAYERSEARCHWIZARDPAGE_H

// TerraLib
#include "../../Config.h"

//STL
#include <memory>

// Qt
#include <QWizardPage>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class LayerSearchWidget;

      class TEQTWIDGETSEXPORT LayerSearchWizardPage : public QWizardPage
      {
        Q_OBJECT

        public:

          LayerSearchWizardPage(QWidget* parent = 0);

          virtual ~LayerSearchWizardPage();

          bool isComplete() const;

          LayerSearchWidget* getSearchWidget() const;


        protected slots:

          void onItemSelectionChanged();

        private:

          std::auto_ptr<LayerSearchWidget> m_widget;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_SEARCH_INTERNAL_LAYERSEARCHWIZARDPAGE_H
