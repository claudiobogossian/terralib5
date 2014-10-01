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
  \file terralib/qt/widgets/rp/ArithmeticOpWizard.h

  \brief A Qt dialog that allows users to run a arithmeticOp operation defined by RP module.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_ARITHMETICOPWIZARD_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_ARITHMETICOPWIZARD_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../../../raster/Raster.h"
#include "../../../rp/ArithmeticOperations.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWizard>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class ArithmeticOpWizardPage;
      class LayerSearchWizardPage;
      class RasterInfoWizardPage;

      /*!
        \class ArithmeticOpWizard

        \brief This class is GUI used to define the arithmeticOp wizard for the RP arithmeticOp operation.
      */
      class TEQTWIDGETSEXPORT ArithmeticOpWizard : public QWizard
      {
        public:
          enum ArithmeticOperationType
          {
            ARITH_OP_TYPE_1,  /* GAIN * A + OFFSET */
            ARITH_OP_TYPE_2,  /* GAIN * (A + B ) + OFFSET */
            ARITH_OP_TYPE_3,  /* GAIN * (A - B ) + OFFSET */
            ARITH_OP_TYPE_4,  /* GAIN * (A * B ) + OFFSET*/
            ARITH_OP_TYPE_5,  /* GAIN * (A / B ) + OFFSET */
            ARITH_OP_TYPE_6,   /* (GAIN * (A - B ) / ( A + B )) + OFFSET */
            ARITH_OP_TYPE_USER_DEFINED
          };

        public:

          ArithmeticOpWizard(QWidget* parent);

          ~ArithmeticOpWizard();

        public:

          virtual bool validateCurrentPage();

          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

          te::map::AbstractLayerPtr getOutputLayer();

        protected:

          void addPages();

          bool execute();

          te::rp::ArithmeticOperations::InputParameters paramsFromOp1();

          te::rp::ArithmeticOperations::InputParameters paramsFromOp2();

          te::rp::ArithmeticOperations::InputParameters paramsFromOp3();

          te::rp::ArithmeticOperations::InputParameters paramsFromOp4();

          te::rp::ArithmeticOperations::InputParameters paramsFromOp5();

          te::rp::ArithmeticOperations::InputParameters paramsFromOp6();

          te::rp::ArithmeticOperations::InputParameters paramsFromOpUserdDef();

        private:

          std::auto_ptr<te::qt::widgets::ArithmeticOpWizardPage> m_arithmeticOpPage;
          std::auto_ptr<te::qt::widgets::LayerSearchWizardPage> m_layerSearchPage;
          std::auto_ptr<te::qt::widgets::RasterInfoWizardPage> m_rasterInfoPage;

          te::map::AbstractLayerPtr m_outputLayer;
      };
    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_ARITHMETICOPWIZARD_H
