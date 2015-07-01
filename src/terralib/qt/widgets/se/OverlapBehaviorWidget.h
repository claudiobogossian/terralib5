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
  \file terralib/qt/widgets/se/OverlapBehaviorWidget.h

  \brief A widget used to build OverlapBehavior element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_OVERLAPBEHAVIORWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_OVERLAPBEHAVIORWIDGET_H

// TerraLib
#include "../Config.h"
#include "../../../se.h"

// Qt
#include <QWidget>

// STL
#include <memory>
#include <string>
#include <vector>

// Forward declaraion
namespace Ui { class OverlapBehaviorWidgetForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class RasterSymbolizer;
  }

  namespace qt
  {
    namespace widgets
    {

      /*!
        \class OverlapBehaviorWidget

        \brief A dialog used to build a OverlapBehaviorWidget element.
      */
      class TEQTWIDGETSEXPORT OverlapBehaviorWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a OverlapBehaviorWidget dialog which is a child of parent, with widget flags set to f. */
          OverlapBehaviorWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~OverlapBehaviorWidget();

          //@}

        public:

          void setOverlapBehavior(te::se::RasterSymbolizer::OverlapBehavior value);

          te::se::RasterSymbolizer::OverlapBehavior getOverlapBehavior() const;

        protected:

          /*! \brief Internal method to initialize the widget (e.g.: color, combos, icons, etc.) */
          void initialize();

          /*! \brief Updates the widget form based on internal element. */
          void updateUi();

        protected slots:

          void onValueChanged(QString value);

        private:

          std::auto_ptr<Ui::OverlapBehaviorWidgetForm> m_ui;             //!< Dialog form.
          std::map<te::se::RasterSymbolizer::OverlapBehavior, QString> m_obNames;

          te::se::RasterSymbolizer::OverlapBehavior m_ob;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_OVERLAPBEHAVIORWIDGET_H
