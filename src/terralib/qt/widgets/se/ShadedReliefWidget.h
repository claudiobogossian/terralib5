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
  \file terralib/qt/widgets/se/ShadedReliefWidget.h

  \brief A widget used to build
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_SHADEDRELIEFWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_SHADEDRELIEFWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

// STL
#include <memory>
#include <string>
#include <vector>

// Forward declaraion
namespace Ui { class ShadedReliefWidgetForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class ShadedRelief;
  }

  namespace qt
  {
    namespace widgets
    {

      /*!
        \class ShadedReliefWidget

        \brief A dialog used to build a ShadedRelief element.
      */
      class TEQTWIDGETSEXPORT ShadedReliefWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a ShadedReliefWidget dialog which is a child of parent, with widget flags set to f. */
          ShadedReliefWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~ShadedReliefWidget();

          //@}

        public:

          void setShadedRelief(const te::se::ShadedRelief* sr);

          te::se::ShadedRelief* getShadedRelief() const;

        protected:

          /*! \brief Internal method to initialize the widget (e.g.: color, combos, icons, etc.) */
          void initialize();

          /*! \brief Updates the widget form based on internal fill element. */
          void updateUi();

        protected slots:

          void onBrightnessToggled(bool flag);

          void onFactorChanged(double value);

        signals:



        private:

          std::auto_ptr<Ui::ShadedReliefWidgetForm> m_ui;             //!< Dialog form.
          te::se::ShadedRelief* m_sr;                                 //!< SE Shaded relief element.

      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_SHADEDRELIEFWIDGET_H
