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
  \file terralib/qt/widgets/rp/ArithmeticOpDialogForm.h

  \brief This file defines a class for a ArithmeticOp Dialog.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_ARITHMETICOPDIALOGFORM_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_ARITHMETICOPDIALOGFORM_H

// TerraLib
#ifndef Q_MOC_RUN
#include "../../../maptools/AbstractLayer.h"
#endif
#include "../Config.h"

#include "../../../rp/ArithmeticOperations.h"
#include "OutputRasterWidget.h"

// STL
#include <memory>

// Qt
#include <QComboBox>
#include <QDialog>


// Forward declaration
namespace Ui { class ArithmeticOpDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class ArithmeticOpDialogForm

        \brief This class is GUI used to define the arithmeticOp parameters for the RP arithmeticOp operation.
      */
      class TEQTWIDGETSEXPORT ArithmeticOpDialogForm : public QDialog
      {
        Q_OBJECT

        public:

          ArithmeticOpDialogForm(QWidget* parent = 0);

          ~ArithmeticOpDialogForm();

          bool isComplete() const;

        public:

          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

          std::string getUserDefinedExpression();

          std::string getExpression();

          te::map::AbstractLayerPtr getOutputLayer();

          std::map<std::string, te::map::AbstractLayer*> getMapLayer();

          std::vector<std::string> getBandsVec();

          te::rp::ArithmeticOperations::InputParameters paramsFromOpUserdDef();

          void closeEvent(QCloseEvent* e);

        public slots:

          void onAddLayerPushButtonClicked();
          void onSearchLayerToolButtonPressed();

          void onSqrtPushButtonClicked();
          void onParRightPushButtonClicked();
          void onParLeftPushButtonClicked();
          void onSinPushButtonClicked();
          void onASinPushButtonClicked();
          void onExpPushButtonClicked();
          void onCosPushButtonClicked();
          void onACosPushButtonClicked();
          void onLogPushButtonClicked();
          void onTanPushButtonClicked();
          void onATanPushButtonClicked();
          void onLnPushButtonClicked();

          void onN0PushButtonClicked();
          void onN1PushButtonClicked();
          void onN2PushButtonClicked();
          void onN3PushButtonClicked();
          void onN4PushButtonClicked();
          void onN5PushButtonClicked();
          void onN6PushButtonClicked();
          void onN7PushButtonClicked();
          void onN8PushButtonClicked();
          void onN9PushButtonClicked();
          void onDotPushButtonClicked();
          void onSignalPushButtonClicked();

          void onDivPushButtonClicked();
          void onMultPushButtonClicked();
          void onSubPushButtonClicked();
          void onAddPushButtonClicked();
          void onCePushButtonClicked();
          void onCPushButtonClicked();
          void onLoadPushButtonClicked();
          void onSavePushButtonClicked();

          void onOkPushButtonClicked();

        signals:

          void addLayer(te::map::AbstractLayerPtr layer);

          void closeTool();

        protected:

          void getRasterBands(QComboBox* layer, int index, QComboBox* band);

          void updateExpressionHistory();

          bool execute();

        private:

          te::qt::widgets::OutputRasterWidget* m_outputWidget;

          std::vector<std::string> m_expressionHistory;

          int m_expressionHistoryCounter;

          std::auto_ptr<Ui::ArithmeticOpDialogForm> m_ui;
          
          std::list<te::map::AbstractLayerPtr> m_layerList;

          te::map::AbstractLayer* m_layer;

          te::map::AbstractLayerPtr m_outputLayer;

          std::map<std::string, te::map::AbstractLayer*> m_mapLayer;

          std::vector<std::string>  m_bandsVec;

          std::string m_operation;

          int m_nLayer;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_ARITHMETICOPDIALOGFORM_H
