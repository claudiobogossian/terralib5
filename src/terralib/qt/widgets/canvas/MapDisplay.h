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
  \file MapDisplay.h

  \brief A widget to control the display of a set of layers.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_MAPDISPLAY_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_MAPDISPLAY_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtCore/QObject>
#include <QtGui/QWidget>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtCore/QTimer>

// TerraLib
#include "Canvas.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../maptools/MapDisplay.h"
#include "../../../geometry/Envelope.h"

// STL
#include <map>

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
      //class Canvas;

      /*!
        \class MapDisplay

        \brief A widget to control the display of a set of layers.

        What is a MapDisplay?
        <br>
        It is an abstraction of .... You can use it to:
        <ul>
        <li>...;</li>
        <li>...;</li>
        </ul>
       */
      class TEQTWIDGETSEXPORT MapDisplay : public QWidget, public te::map::MapDisplay
      {
        Q_OBJECT

        public:

           /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs an empty label.

            \param w
            \param h
            \param parent
            \param f
           */
          MapDisplay(int w, int h, QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~MapDisplay();

          //@}

          /** @name MapDisplay Methods
           *  Methods to configure the MapDisplay.
           */
          //@{

          /*!
            \brief It will set the align rendering of objects into the map display. Just successive drawings will be affected by this modification.

            \param h The new horizontal align.
            \param v The new vertical align.

            \note It will not automatically redraw the objects, you must explicit call the setExtent method.
           */
          void setAlign(const te::map::AlignType& h, const te::map::AlignType& v);

          /*!
            \brief It sets the world visible area. If the given area is not proportinal to the device width and height,
                   the MapDisplay will change it in order to preserve the aspect ratio.
                   Just successive calls to the draw method will be affected.

            \param e The world visible area. It coordinates must be in the Map Display SRS.

            \todo Pensar se ter um metodo chamado getBestFit seria mais adequado para que o setExtent fosse mais burrinho, isto e, so fizesse o que manda e nao pensasse qual a melhro proporcao.
           */
          virtual void setExtent(const te::gm::Envelope& e);

          /*!
            \brief It gets the box if it exists. Otherwise, it calculates the box through the related dataset and updates the layer, then returns the box.

            \param al The abstract layer.
           */
          te::gm::Envelope getLayerExtent(te::map::AbstractLayer* al);

          /*!
            \brief It return the display pixmap.

            \return The display pixmap.

            \note The caller of this method will NOT take the ownership of the returned pixmap.           
           */
          QPixmap* getDisplayPixmap();

          /*!
            \brief It sets repaint state.

            \param s true:update.;  false: not update
           */
          void setRepaint(bool);

          //@}

          /** @name Drawing Methods
           *  Methods to show or hide layers in the display.
           */
          //@{

          /*!
            \brief It displays the layers.

            \note It may need to retrieve data in order top display the layers.
           */
          virtual void draw();

          //@}

          /** @name Event Handlers
           *  Implementation of Qt event handlers.
           */
          //@{

          /*!
            \brief He assembles the final image and draws the widget.

            \param e The event received by this widget.
           */
          virtual void paintEvent(QPaintEvent* e);

          /*!
            \brief This event handler receives widget resize events wich are passed in the event parameter.

            \param e The resize event.
           */
          void resizeEvent(QResizeEvent* e);   

          /*!
            \brief It Shows a time cursor.
           */
          void setWaitCursor();

          /*!
            \brief It shows the default cursor.
           */
          void unsetWaitCursor();

        signals:

          /*!
            \brief This signal is emitted when the mapDisplay is resized.

            \param s The new size.
           */
          void sizeChanged(QSize s);


          //@}

        protected:

          /*!            
          \brief It retrieves an associated canvas to the layer.

          \param layer The layer for wich we want to check an associated canvas.

          \return A canvas associated to the layer.
          */
          virtual te::qt::widgets::Canvas* getCanvas(te::map::AbstractLayer* layer);

          /*!            
          \brief It sets an associated canvas to the layer.

          \param layer The layer for wich we want to set an associated canvas.

          \return A canvas associated to the layer.
          */
          virtual void setCanvas(te::map::AbstractLayer* layer);

          /*!
          \brief It assembles a list of layers from layer tree and draw them.

          \param layerTree The layer tree.

          \note The visible layers of the tree will be drawn from the bottom up.
                The top layer will be the last to be drawn. 
           */
          virtual void draw(te::map::AbstractLayer* layerTree);

          /*!
            \brief It sequentially draws the layers of list (up to bottom).

            \param layerList List of layers to be drawn in order.

            \note The layers of this list are always leaves (they are never nodes).
                  The last layer of the list will be the last to be drawn.
           */
          void draw(std::list<te::map::AbstractLayer*>& layerlist);

          /*!
            \brief It fits the layers.

            \param layerList List of layers to be drawn in order.

            \note The layers of this list are always leaves (they are never nodes).
           */
          void fit(std::list<te::map::AbstractLayer*>& layerList);

          /*!
            \brief It assembles a list of visible layers from the tree.

            \param al The root folder layer.

            \param layerList List of layers to be assembled.

            \note The layers of this list are always leaves (they are never nodes).
           */
          void mountLayerList(te::map::AbstractLayer* al, std::list<te::map::AbstractLayer*>& layerList);

          /*!            
            \brief Recalculates the extension of all visible layers.

            \return The envelope of all visible layers.
          */
          te::gm::Envelope getAllExtent();

          /*!            
            \brief Recalculates the extension of all visible layers.

            \layerList List of layers to be calculated.

            \return The envelope of all visible layers.
          */
          te::gm::Envelope getAllExtent(std::list<te::map::AbstractLayer*>& layerList);

          void contextMenuEvent(QContextMenuEvent*);

        protected slots:

          /*!            
            \brief Recalculates the extension to fit all the layers and then draw.
          */
          void fitAllLayersSlot();

          /*!            
            \brief It sets the new SRS.
          */
          virtual void setSRIDSlot();

          /*!            
            \brief It slows the drawing at 350 ms. It is used in resize event operations.
                   The call happens when there is a pause of 350ms.
          */
          void drawTimerSlot();

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

          QPixmap* m_displayPixmap;         //!< This pixmap will be the result of all canvas pixmap drawing, i. e., the result of drawing all visible layers.
          QColor  m_backgroundColor;        //!< background color
          bool m_resize;                    //!< flag to draw with zoom due to the resize in progress
          QRect m_resizeRec;                //!< rectangle used for display resize
          QRect m_resizeWRec;               //!< rectangle used for display resize
          bool m_repaint;                   //!< repaint control.
          QTimer* m_timer;                  //!< timer to execute zoom on resize action.
          te::map::AlignType m_hAlign;      //!< The display horizontal align.
          te::map::AlignType m_vAlign;      //!< The display vertical align.
          std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*> m_layerCanvasMap; //!< A map: layer -> canvas.
          te::gm::Envelope m_envelope;      //!< Envelope.
          QMenu* m_menu;                    //!< Menu.
          QAction* m_fitAllLayersAction;    //!< Action to fit all layers.
          QAction* m_srsAction;             //!< Action to change srid.
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_MAPDISPLAY_H

