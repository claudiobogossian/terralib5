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

#ifndef __TERRALIB_VP_INTERNAL_BASICGEOGRAPHICOPWIZARDPAGE_H
#define __TERRALIB_VP_INTERNAL_BASICGEOGRAPHICOPWIZARDPAGE_H

// TerraLib
#include "../../maptools/AbstractLayer.h"
#include "../../qt/widgets/utils/DoubleListWidget.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QtGui/QWizardPage>

// Forward declaration
namespace Ui { class BasicGeographicOpWizardPageForm; }

namespace te
{
  namespace vp
  {
    //class RasterNavigatorWidget;

    /*!
      \class BasicGeographicWizardPage

      \brief This class is GUI used to define the basic geographic operator parameters for the VP operation.
    */
    class TEVPEXPORT BasicGeographicOpWizardPage : public QWizardPage
    {
      Q_OBJECT

      public:

        BasicGeographicOpWizardPage(QWidget* parent = 0);

        ~BasicGeographicOpWizardPage();

      public:

        /*!
          \brief This method is used to set the selected layer for segmenter operation
            
          \param layer The layer ptr 

          \note This layer MUST HAVE a valid raster object.
        */
        void setLayer(te::map::AbstractLayerPtr layer);

        te::map::AbstractLayerPtr getLayer();

        std::vector<std::string> getSelectedProps();

      public slots:

      protected:

        void loadAttributes();

        //void listBands();

      private:

        std::auto_ptr<Ui::BasicGeographicOpWizardPageForm> m_ui;
        std::auto_ptr<te::qt::widgets::DoubleListWidget> m_widget;
        te::map::AbstractLayerPtr m_layer;
        std::vector<std::string> m_props;
    };

  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_BASICGEOGRAPHICOPWIZARDPAGE_H
