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
  \file terralib/qt/widgets/se/BasicStrokeDialog.h

  \brief A dialog used to build a basic stroke element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICSTROKEDIALOG_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICSTROKEDIALOG_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QDialog>

// STL
#include <memory>

// Forward declaraion
namespace Ui { class BasicStrokeDialogForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class Stroke;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class BasicStrokeWidget;

      /*!
        \class BasicStrokeDialog

        \brief A dialog used to build a basic stroke element.
      */
      class TEQTWIDGETSEXPORT BasicStrokeDialog : public QDialog
      {
        Q_OBJECT

        public:

          BasicStrokeDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~BasicStrokeDialog();

        public:

          te::se::Stroke* getStroke() const;

        private:

          std::auto_ptr<Ui::BasicStrokeDialogForm> m_ui;

          te::qt::widgets::BasicStrokeWidget* m_strokeWidget;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICSTROKEDIALOG_H
