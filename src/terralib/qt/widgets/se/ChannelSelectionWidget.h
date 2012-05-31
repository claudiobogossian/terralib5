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
  \file terralib/qt/widgets/se/ChannelSelectionWidget.h

  \brief A widget used to build
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_CHANNELSELECTIONWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_CHANNELSELECTIONWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QWidget>

// STL
#include <memory>
#include <string>
#include <vector>

// Forward declaraion
namespace Ui { class ChannelSelectionWidgetForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class ChannelSelection;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class SelectedChannelWidget;


      /*!
        \class ChannelSelectionWidget

        \brief A dialog used to build a ChannelSelection element.
      */
      class TEQTWIDGETSEXPORT ChannelSelectionWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a ChannelSelectionWidget dialog which is a child of parent, with widget flags set to f. */
          ChannelSelectionWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~ChannelSelectionWidget();

          //@}

        public:



        protected:

          /*! \brief Updates the widget form based on internal mark element. */
          void updateUi();

        protected slots:



        signals:


        private:

          std::auto_ptr<Ui::ChannelSelectionWidgetForm> m_ui;             //!< Dialog form.
          te::qt::widgets::SelectedChannelWidget* m_redSCWidget;          //!< Selected Channel Widget form for red band.
          te::qt::widgets::SelectedChannelWidget* m_greenSCWidget;        //!< Selected Channel Widget form for green band.
          te::qt::widgets::SelectedChannelWidget* m_blueSCWidget;         //!< Selected Channel Widget form for blue band.
          te::qt::widgets::SelectedChannelWidget* m_monoSCWidget;         //!< Selected Channel Widget form for mono band.

      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_CHANNELSELECTIONWIDGET_H
