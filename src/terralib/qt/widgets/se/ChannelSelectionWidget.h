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
#include <QWidget>

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
    class SelectedChannel;
  }

  namespace rst
  {
    class BandProperty;
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

          void setChannelSelection(const te::se::ChannelSelection* cs);

          te::se::ChannelSelection* getChannelSelection() const;

          /*! 
            \brief Sets the band information

            \param p Vector with band property

            \note The class will NOT take the ownership of the pointer.
          */
          void setProperty(std::vector<te::rst::BandProperty*>& p);

        protected:


          /*! \brief Internal method to initialize the widget (e.g.: color, combos, icons, etc.) */
          void initialize();

          /*! \brief Updates the widget form based on internal mark element. */
          void updateUi();

        protected slots:

          void onRGBColorSelected();
          void onGrayScalelSelected();

          void onRedChannelSelected();
          void onGreenChannelSelected();
          void onBlueChannelSelected();
          void onMonoChannelSelected();
          

          void onSelectedChannelChanged();

        signals:


        private:

          std::auto_ptr<Ui::ChannelSelectionWidgetForm> m_ui;             //!< Dialog form.
          te::qt::widgets::SelectedChannelWidget* m_sCWidget;             //!< Selected Channel Widget.

          te::se::SelectedChannel* m_scRed;                               //!< SE Selected Channel element for red band.
          te::se::SelectedChannel* m_scGreen;                             //!< SE Selected Channel element for red band.
          te::se::SelectedChannel* m_scBlue;                              //!< SE Selected Channel element for red band.
          te::se::SelectedChannel* m_scMono;                              //!< SE Selected Channel element for red band.

          te::se::ChannelSelection* m_cs;                                 //!< SE Channel Selection element.

          std::vector<te::rst::BandProperty*> m_bands;                             //!< A vector of bands description.

      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_CHANNELSELECTIONWIDGET_H
