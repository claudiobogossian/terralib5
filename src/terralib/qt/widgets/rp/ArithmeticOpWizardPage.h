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
  \file terralib/qt/widgets/rp/ArithmeticOpWizardPage.h

  \brief This file defines a class for a ArithmeticOp Wizard page.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_ARITHMETICOPWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_ARITHMETICOPWIZARDPAGE_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QComboBox>
#include <QWizardPage>


// Forward declaration
namespace Ui { class ArithmeticOpWizardPageForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class ArithmeticOpWizardPage

        \brief This class is GUI used to define the arithmeticOp parameters for the RP arithmeticOp operation.
      */
      class TEQTWIDGETSEXPORT ArithmeticOpWizardPage : public QWizardPage
      {
        Q_OBJECT

        public:

          ArithmeticOpWizardPage(QWidget* parent = 0);

          ~ArithmeticOpWizardPage();

          bool isComplete() const;

        public:

          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

          int getOperationType();

          te::map::AbstractLayerPtr getLayerRasterA();

          int getLayerBandA();

          te::map::AbstractLayerPtr getLayerRasterB();

          int getLayerBandB();

          double getGainValue();

          double getOffSetValue();

          bool normalize();

          std::string getUserDefinedExpression();

        public slots:

          void layerComboBoxActivated(int index);
          void layerAComboBoxActivated(int index);
          void layerBComboBoxActivated(int index);

          void leftBracketToolButtonClicked();
          void rightBracketToolButtonClicked();
          void plusToolButtonClicked();
          void minusToolButtonClicked();
          void multiToolButtonClicked();
          void divToolButtonClicked();

          void addValueOpToolButtonClicked();
          void addRasterOpToolButtonClicked();

          void undoToolButtonClicked();
          void redoToolButtonClicked();
          void clearToolButtonClicked();
          void validateToolButtonClicked();

        protected:

          void getRasterBands(QComboBox* layer, int index, QComboBox* band);

          void updateExpressionHistory();

        private:

          std::vector<std::string> m_expressionHistory;

          int m_expressionHistoryCounter;

          std::auto_ptr<Ui::ArithmeticOpWizardPageForm> m_ui;
          
          std::list<te::map::AbstractLayerPtr> m_layerList;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_ARITHMETICOPWIZARDPAGE_H
