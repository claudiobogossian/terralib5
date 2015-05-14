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
  \file terralib/qt/widgets/rp/TiePointLocatorParametersWidget.h

  \brief This file has the TiePointLocatorParametersWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_TIEPOINTLOCATORPARAMETERSWIDGET_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_TIEPOINTLOCATORPARAMETERSWIDGET_H

// TerraLib
#include "../../../rp/TiePointsLocator.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWidget>

namespace Ui { class TiePointLocatorParametersWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class TiePointLocatorParametersWidget

        \brief This class is used to define a widget for tie point parameters acquirement.
      */
      class TEQTWIDGETSEXPORT TiePointLocatorParametersWidget : public QWidget
      {
        Q_OBJECT

        public:

          TiePointLocatorParametersWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~TiePointLocatorParametersWidget();

        public:

          void setMosaicLayout();

          void setRescaleFactor(double value);

          std::string getTransformationName();

          te::rp::TiePointsLocator::InputParameters getTiePointInputParameters();

        protected:

          /*! \brief Fill interface widget with initial values. */
          void startAdvancedOptions();

          /*! \brief Update tie point values with  advanced options changed values. */
          void updateAdvancedOptions();

        protected:

          std::auto_ptr<Ui::TiePointLocatorParametersWidgetForm> m_ui;

          te::rp::TiePointsLocator::InputParameters m_inputParameters;   //!< The current parameters.
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_TIEPOINTLOCATORPARAMETERSWIDGET_H

