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
  \file terralib/qt/widgets/rp/LoadArithmeticOpDialog.h

  \brief This file defines a class for a ArithmeticOp Dialog.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_LOADARITHMETICOPDIALOG_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_LOADARITHMETICOPDIALOG_H

// TerraLib
#ifndef Q_MOC_RUN
#include "../../../maptools/AbstractLayer.h"
#endif
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QComboBox>
#include <QDialog>
#include <QTableWidgetItem>


// Forward declaration
namespace Ui { class LoadArithmeticOpDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class LoadArithmeticOpDialog

        \brief This class is GUI used to define the arithmeticOp parameters for the RP arithmeticOp operation.
      */
      class TEQTWIDGETSEXPORT LoadArithmeticOpDialog : public QDialog
      {
        Q_OBJECT

        public:

          LoadArithmeticOpDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~LoadArithmeticOpDialog();

        public:

          bool loadOperations();

          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

          std::string getExpression();

          std::map<std::string, te::map::AbstractLayer*> getLayers();

          std::string getOperation();

        public slots:

        void onOperationComboBoxActivated(QString operation);

        void onLayerComboBoxActivated(int index);

        void onOkPushButtonClicked();

        private:

          std::auto_ptr<Ui::LoadArithmeticOpDialogForm> m_ui;
          
          std::list<te::map::AbstractLayerPtr> m_layerList;

          std::vector<te::map::AbstractLayerPtr> m_layers;

          std::map<std::string, te::map::AbstractLayer*> m_mapLayer;

          std::vector<QComboBox*> m_layersComboBox;

          std::vector<QComboBox*> m_bandsComboBox;

          std::string m_operationExt;

          std::string m_operation;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_LOADARITHMETICOPDIALOG_H
