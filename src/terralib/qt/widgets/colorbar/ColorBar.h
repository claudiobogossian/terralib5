/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/qwt/ColorBar.h

  \brief The QWidget of color bar.
*/

#ifndef __TERRALIB_QT_QWT_INTERNAL_COLORBAR_H_
#define __TERRALIB_QT_QWT_INTERNAL_COLORBAR_H_

//QWT

#include <qwt_scale_widget.h>

//STL
#include <map>

//TerraLib
#include "../Config.h"

class QMenu;
class QAction;
class QWidgetAction;

namespace te
{
  namespace color
  {
    class ColorBar;
  }

  namespace qt
  {
    namespace widgets
    {
      class ColorPickerToolButton;

      namespace colorbar
      {
        /*!
          \class ColorBar

          \brief It QWidget implementation of color bar.

            It is used to construct color bar.

          \ingroup widgets
        */
        class TEQTWIDGETSEXPORT ColorBar: public QwtScaleWidget
        {
        
          Q_OBJECT
        
          public:

            /** @name Initializer Methods
             *  Methods related to instantiation and destruction.
             */
            //@{

            /*!
            \brief It initializes a new ColorBar.

            \param parent
            */

            ColorBar(QWidget* parent = 0);

            /*! 
            \brief Destructor */

            ~ColorBar();

            //@}

          public:

            /** @name Accessor methods
            *  Methods used to get or set properties.
            */
            //@{

            /*!
            \brief Sets scale of color bar visibility.

            \param flag Boolean value to set the scale of color bar visibility.

            \note TRUE is the defaul behavior.
            */
            void setScaleVisible(bool flag);

            /*!
            \brief Sets the height of colobar.

            \param Value of height.
            */
            void setHeight(int value);

            /*!
            \brief It sets the min e max value of colorbar.

            \param Minimum value.
            \param Maximum value.
            */
            void setInterval(double min, double max);

            /*!
            \brief It sets the color bar.

            \param te::color::colorBar to paint in the colorbar.

            \note This class takes the ownership of the colorbar pointer.
            */
            void setColorBar(te::color::ColorBar* cb);

            /*!
            \brief It sets the precision of mouse click.

            \param double precision of click.
            */

            void setClickPrecision(double precision);

            /*!
            \brief It returns the colorbar.

            \return Current color bar.
            */
            te::color::ColorBar* getColorBar();

            //@}

          protected:
            /** @name Private methods
            *  Methods used to manange colors and events.
            */
            //@{

            /*!
            \brief Sets the transformation and the scale of colorbar values.
            */
            void setScaleEngine();

            /*!
            \brief It build and paint the colorbar.
            */
            void buildColorBar();

            /*!
            \brief It returns the pin based on mouse click and the click precision

            \param pos Mouse click X on toolbar

            \return Pin found or -1, if not found.
            */
            double getPin(int pos);

            /*!
            \brief It convert a mouse position to a toolbar position

            \param pos Mouse click X on toolbar

            \return Toolbar position
            */
            double convert2toolbarPos(int pos);

            /*!
            \brief Inherit of QEvent, receive events to paint e update widgets.

            \param QPaintEvent
            */
            virtual void paintEvent(QPaintEvent* e);

            /*!
            \brief It waits a pressEvent from mouse to do something.

            \param QMouseEvent
            \note In this case, it treat the remove colorStops funciontion
            \note and sets the start position to move stops.
            */
            virtual void mousePressEvent(QMouseEvent* e);

            /*!
            \brief It waits a releaseEvent from mouse to do something.

            \param QMouseEvent
            \note In this case, it treat the finalization of move colorStops.
            */
            virtual void mouseReleaseEvent(QMouseEvent* e);

            /*!
            \brief It waits a moveEvent from mouse to do something.

            \param QMouseEvent
            \note In this case, it treat the moce of colorStops.
            */
            virtual void mouseMoveEvent(QMouseEvent* e);

            /*!
            \brief It waits the wheel from mouse to do something.

            \param QWheelEvent
            \note In this case, it change the color lightness.
            */
            virtual void wheelEvent(QWheelEvent* e);

            //@}

          protected slots:
          
            /*!
            \brief It add a colorbar pin.
            */
            void addPin();

            /*!
            \brief It remove a colorbar pin.
            */
            void removePin();

            /*!
            \brief It edit a colorbar pin.
            */
            void editPin();

            /*!
            \brief It changes the pins positions for an equal distribution.
            */
            void equalStep();

          signals:

            void colorBarChanged();


          protected:

            te::color::ColorBar* m_colorBar;       //!< Color map information
          
            QwtInterval m_interval;                //!< Color vector.

            int m_height;                          //!< Height of colobar.

            double m_initialXPos;                  //!< Position initial to move color.

            double m_clickPrecision;               //!< Precision of click to get a color stop.

            double m_currentPinPos;                //!< Value of the current Pin position.

            QMenu* m_colorBarMenu;                 //!< Popup menu for Pin on color bar.

            QMenu* m_pinMenu;                      //!< Popup menu for Pin on color bar.

            te::qt::widgets::ColorPickerToolButton* m_colorBarPicker; //!< Color picker for colorbar menu.

            te::qt::widgets::ColorPickerToolButton* m_pinPicker; //!< Color picker for pin menu.

            QWidgetAction* m_addPinAction;         //!< Add Pin Action

            QWidgetAction* m_editPinAction;        //!< Edit Pin Action

            QAction* m_removePinAction;            //!< Remove Pin Action

            QAction* m_equalStepAction;            //!< Equal Step Pin Action

        };
      } // end namespace qwt
    }
  } // end namespace qt
}
#endif // __TERRALIB_QT_QWT_INTERNAL_COLORBAR_H_