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
  \file terralib/qt/widgets/rp/MaskDialog.h

  \brief This file defines a class for a MaskDialog.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_MASKDIALOG_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_MASKDIALOG_H

// TerraLib
#include "../Config.h"

//STL
#include <memory>

//Boost
#include <boost/numeric/ublas/matrix.hpp>

// Qt
#include <QDialog>
#include <QLineEdit>

namespace Ui { class MaskDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class MaskDialog

        \brief This class is a dialog to create a user defined mask.
      */
      class TEQTWIDGETSEXPORT MaskDialog : public QDialog
      {
        Q_OBJECT

        public:

          MaskDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~MaskDialog();

        public:

          void setMaskSize(int size, int defaultValue);

          void setMaskSize(boost::numeric::ublas::matrix<double> matrix);

          boost::numeric::ublas::matrix<double> getMatrix();

        public slots:

          void onOkPushButtonClicked();

        private:

          std::auto_ptr<Ui::MaskDialogForm> m_ui;

          std::vector< std::vector<QLineEdit*> > m_matrix;

          boost::numeric::ublas::matrix<double> m_window; //!< User defined convolution window.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_MASKDIALOG_H
