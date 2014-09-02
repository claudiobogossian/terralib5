/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/canvas/MapDisplay.h

  \brief A Qt4 widget to control the display of a set of layers.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_MAPDISPLAY_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_MAPDISPLAY_H

// TerraLib
#include "../../../maptools/MapDisplay.h"
#include "../../../srs.h"
#include "../Config.h"

// Qt
#include <QWidget>

// STL
#include <map>

// Forward declarations
class QTimer;
class QGraphicsView;

namespace te
{
// Forward declarations
  namespace gm { class Envelope; }
  namespace map { class AbstractLayer; }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
    class Canvas;

      /*!
        \class MapDisplay

        \brief A widget to control the display of a set of layers.

        \ingroup widgets

        \sa te::map::AbstractMapDisplay, te::map::MapDisplay
      */
      class TEQTWIDGETSEXPORT MapDisplay : public QWidget, public te::map::MapDisplay
      {
        Q_OBJECT

        public:

          /*!
            \enum ResizePolicy

            \brief Defines the resize policy for map display.
          */
          enum ResizePolicy
          {
            Fixed,  /*!< Document-me! */
            Cut,    /*!< Document-me! */
            Center, /*!< Document-me! */
          };

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs an empty map display with the given dimensions which is a child of parent, with widget flags set to f.

            \param size The map display size.
            \param parent The widget's parent.
            \param f Widget window flags.
          */
          MapDisplay(const QSize& size, QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          virtual ~MapDisplay();

          //@}

          /** @name AbstractMapDisplay Methods */
          //@{

          virtual void changeData(te::map::AbstractLayerPtr, int nsrid = TE_UNKNOWN_SRS);

          virtual void setExtent(te::gm::Envelope& e, bool doRefresh = true);

          virtual void refresh();

          unsigned int getWidth() const;

          unsigned int getHeight() const;

          double getWidthMM() const;

          double getHeightMM() const;

          virtual void setSRID(const int& srid, bool doRefresh = true);

          //@}

          /*!
            \brief It returns the map display pixmap.

            \return The map display pixmap.

            \note The caller of this method will NOT take the ownership of the returned pixmap.
          */
          virtual QPixmap* getDisplayPixmap() const;

          /*!
            \brief It returns the map display draft pixmap.

            \return The map display draft pixmap.

            \note This pixmap can be used to draw some feedback on map display.
            \note The caller of this method will NOT take the ownership of the returned pixmap.
          */
          virtual QPixmap* getDraftPixmap() const;

          /*!
            \brief Sets the resize policy to this map display.

            \param policy The resize policy.
          */
          virtual void setResizePolicy(const ResizePolicy& policy);

          /*!
            \brief Sets the timeout interval in milliseconds to redraw on resize event.

            \param msec The timeout interval in milliseconds.
          */
          virtual void setResizeInterval(int msec);

          /*!
            \brief Transforms the given point, in screen coordinates, to a point in world coordinates.

            \param p A point in screen coordinates.

            \return The point in world coordinates.

            \note This method will return a null point if the transform can not be done.
          */
          virtual QPointF transform(const QPointF& p);

          /*!
            \brief Gets the map display background color

            \return The map display background color.
          */
          virtual QColor getBackgroundColor();

          /*!
            \brief Sets the map display background color

            \param color The color.
          */
          virtual void setBackgroundColor(const QColor& color);

          /*! \brief Returns if the map display is drawing. */
          bool isDrawing() const;

        protected:

          /*!
            \brief It displays the given layer.

            \param layer The layer that will be drawn.
            \param painter The painter that will composed the draw result.

            \note This method is called recursively for each child of the given layer.
            \note It may need to retrieve data in order top display the layers.
          */
          virtual void draw(te::map::AbstractLayer* layer, QPainter& painter);

          /*!
            \brief It retrieves an associated canvas to the given layer.

            \param layer The layer for wich we want to get an associated canvas.

            \note If there is not a canvas associated to the given layer, one will be created.

            \return A canvas associated to the layer.
          */
          virtual te::qt::widgets::Canvas* getCanvas(te::map::AbstractLayer* layer, int type = QInternal::Pixmap);

          /*!
            \brief It resizes all canvas of map display.
          */
          virtual void resizeAllCanvas();

          /*!
            \brief It adjusts the map display extent based on resize policy.

            \param oldSize The size before the resize. 
            \param size The size after the resize.
          */
          virtual void adjustExtent(const QSize& oldSize, const QSize& size);

          /*!
            \brief It assembles the final image and draws the widget.

            \param e The event received by this widget.
          */
          virtual void paintEvent(QPaintEvent* e);

          /*!
            \brief This event handler receives widget resize events wich are passed in the event parameter.

            \param e The resize event.
          */
          virtual void resizeEvent(QResizeEvent* e);

        protected slots:

          virtual void onResizeTimeout();

        signals:

          /*! This signal is emitted when the map display extent changed. */
          void extentChanged();

          /*! this signal is issued to others draw on this display. */
          void displayPaintEvent(QPainter*);

          /*! This signal is sent to others know that the projection was changed. */
          void displaySridChanged();

          ///*! This signal is sent to others know that is drag enter event. */
          //void displayDragEnterEvent(QDragEnterEvent*);
          //void displayDragMoveEvent(QDragMoveEvent*);
          ///*! This signal is sent to others know that is drop event. */
          //void displayDropEvent(QDropEvent*);

        private:

          /** @name Copy Constructor and Assignment Operator
           *  Copy constructor and assignment operator not allowed.
           */
          //@{

          /*!
            \brief Copy constructor not allowed.

            \param rhs The right-hand-side copy that would be used to copy from.
          */
          MapDisplay(const MapDisplay& rhs);

          /*!
            \brief Assignment operator not allowed.

            \param rhs The right-hand-side copy that would be used to copy from.

            \return A reference to this object.
          */
          MapDisplay& operator=(const MapDisplay& rhs);

          //@}

        protected:

          QPixmap* m_displayPixmap;    //!< This pixmap will be the result of all canvas pixmap drawing, i. e., the result of drawing all visible layers.
          QPixmap* m_draftPixmap;      //!< The draft pixmap can be used to draw some feedback on map display.
          QColor  m_backgroundColor;   //!< Background color.
          ResizePolicy m_resizePolicy; //!< Resize policy for this map display.
          QSize m_oldSize;             //!< Stores the last size of map display on start resize event.
          QTimer* m_timer;             //!< Timer to execute redraw on resize action.
          int m_interval;              //!< Timeout interval in milliseconds to redraw.
          bool m_isDrawing;            //!< A flag that indicates if the map display is drawing.
          std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*> m_layerCanvasMap; //!< A map that associates a layer to a canvas.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_MAPDISPLAY_H
