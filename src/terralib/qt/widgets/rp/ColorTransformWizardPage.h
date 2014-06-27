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
  \file terralib/qt/widgets/rp/ColorTransformWizardPage.h

  \brief This file defines a class for a ColorTransform Wizard page.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_COLORTRANSFORMWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_COLORTRANSFORMWIZARDPAGE_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QComboBox>
#include <QWizardPage>


// Forward declaration
namespace Ui { class ColorTransformWizardPageForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class ColorTransformWizardPage

        \brief This class is GUI used to define the colorTransform parameters for the RP colorTransform operation.
      */
      class TEQTWIDGETSEXPORT ColorTransformWizardPage : public QWizardPage
      {
        Q_OBJECT

           enum ColorTransformTypes
          {
            COLORTRANSFORM_RGB2IHS,
            COLORTRANSFORM_IHS2RGB
          };

        public:

          ColorTransformWizardPage(QWidget* parent = 0);

          ~ColorTransformWizardPage();

          bool isComplete() const;

        public:

          bool isRGB2IHS();

          bool isIHS2RGB();

          te::rst::Raster* getRGBRaster();

          unsigned int getRGBRBand();
          unsigned int getRGBGBand();
          unsigned int getRGBBBand();

          double getRGBMinValue();
          double getRGBMaxValue();

          te::rst::Raster* getIHSRaster();

          unsigned int getIHSIBand();
          unsigned int getIHSHBand();
          unsigned int getIHSSBand();

          double getIHSMinValue();
          double getIHSMaxValue();


          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

        public slots:

          void rgbRComboBoxActivated(int index);
          void rgbGComboBoxActivated(int index);
          void rgbBComboBoxActivated(int index);

          void ihsIComboBoxActivated(int index);
          void ihsHComboBoxActivated(int index);
          void ihsSComboBoxActivated(int index);

        protected:

          void fillColorTransformTypes();

          void getRasterBands(QComboBox* layer, int index, QComboBox* band);

        private:

          std::auto_ptr<Ui::ColorTransformWizardPageForm> m_ui;
          
          std::list<te::map::AbstractLayerPtr> m_layerList;

      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_COLORTRANSFORMWIZARDPAGE_H
