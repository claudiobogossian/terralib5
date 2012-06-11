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
  \file terralib/qt/widgets/se/ContrastEnhancementWidget.h

  \brief A widget used to build ContrastEnhancement element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_CONTRASTENHANCEMENTWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_CONTRASTENHANCEMENTWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QWidget>

// STL
#include <memory>
#include <string>
#include <vector>

// Forward declaraion
namespace Ui { class ContrastEnhancementWidgetForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class ContrastEnhancement;
  }

  namespace qt
  {
    namespace widgets
    {

      /*!
        \class ContrastEnhancementWidget

        \brief A dialog used to build a ContrastEnhancementWidget element.
      */
      class TEQTWIDGETSEXPORT ContrastEnhancementWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a ContrastEnhancementWidget dialog which is a child of parent, with widget flags set to f. */
          ContrastEnhancementWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~ContrastEnhancementWidget();

          //@}

        public:



        protected:

          /*! \brief Updates the widget form based on internal mark element. */
          void updateUi();

        protected slots:



        signals:



        private:

          std::auto_ptr<Ui::ContrastEnhancementWidgetForm> m_ui;             //!< Dialog form.

      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_CONTRASTENHANCEMENTWIDGET_H