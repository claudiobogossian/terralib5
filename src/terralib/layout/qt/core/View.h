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
  \file View.h
   
  \brief Class representing the view. This view is child of QGraphicsView, part of Graphics View Framework. 
  The View object is a widget where the items in the scene are drawn. 
  It is responsible for presentation and interaction between the screen events and the scene. 
  Manages interactive functions and intercepts all mouse or keyboard events. 
  The coordinate system of this representation is pixel and your point 0.0 is in the top left(Screen coordinate system).
  The default settings of the layout module are in the object Context (singleton).

  \ingroup layout
*/

#ifndef  __TERRALIB_LAYOUT_INTERNAL_VIEW_H 
#define __TERRALIB_LAYOUT_INTERNAL_VIEW_H

// TerraLib
#include "../../core/AbstractView.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Coord2D.h"
#include "Scene.h"
#include "../outside/PageSetupOutside.h"
#include "../outside/SystematicScaleOutside.h"
#include "../item/MovingItemGroup.h"
#include "propertybrowser/MenuBuilder.h"

// STL
#include <string>

// Qt
#include <QGraphicsView>
#include <QMenu>
#include <QImage>
#include <QCursor>

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;
class QString;
class QHideEvent;
class QCloseEvent;
class QGraphicsItemGroup;
class QLineF;
class QContextMenuEvent;
class QEvent;

namespace te
{
  namespace layout
  {
    class VisualizationArea;
    class AbstractLayoutTool;
    class HorizontalRuler;
    class VerticalRuler;
    class EnumType;
    class WaitView;
    class ContextObject;

  /*!
    \brief Class representing the view. This view is child of QGraphicsView, part of Graphics View Framework. 
    The View object is a widget where the items in the scene are drawn. 
    It is responsible for presentation and interaction between the screen events and the scene. 
    Manages interactive functions and intercepts all mouse or keyboard events. 
    The coordinate system of this representation is pixel and your point 0.0 is in the top left(Screen coordinate system).
    The default settings of the layout module are in the object Context (singleton).
    
    \ingroup layout

    \sa te::layout::AbstractView
  */
    class TELAYOUTEXPORT View : public QGraphicsView, public AbstractView
    {
      Q_OBJECT //for slots/signals

      public:

    /*!
          \brief Constructor
        */ 
        View(QWidget* widget = (QWidget*)0);

    /*!
          \brief Destructor
        */ 
        virtual ~View();

    /*!
          \brief Initialization method that configures the View and sets the transformation matrix of the scene in the View object.    
        */
        virtual void config();     

    /*!
          \brief Close all windows that have been instantiated by this View. Ex .: Page Settings     
        */
        virtual void closeOutsideWindows();

    /*!
          \brief Create snapshot of the View current state.

      \return image
        */
        virtual QImage createImage();

    /*!
          \brief Reset to initial configuration made in the config() method.    
        */
        virtual void resetView();

    /*!
          \brief Method that sets View object for the Pan function.  
        */
        virtual void pan();

    /*!
          \brief Method that sets View object for the Zoom Area function.  
        */
        virtual void zoomArea();

    /*!
          \brief Method that sets View object for the Zoom Out function.  
        */
        virtual void zoomOut();

    /*!
          \brief Method that sets View object for the Print function.  
        */
        virtual void print();

    /*!
          \brief Reset to initial configuration made in the config() method. Reset to default zoom factor. 
        */
        virtual void recompose();

        /*!
          \brief Method that sets View object for the Print function.  
        */
        virtual void exportToPDF();

        /*!
      \brief This method allows to refresh all properties of the selected item.
    */
        virtual void refreshAllProperties();

        /*!
          \brief Responsible method for verifying changes made in Context outside the View object and call corresponding actions. 
          Ex.: Action Pan called in toolbar changed Context to Pan mode, then View object call method to do it.
        */
        virtual void changeMode(EnumType* newMode);

        /*!
          \brief Reset the view interaction with the default settings, cursor, viewport update and no current tools.
        */
        virtual void resetDefaultConfig();

        virtual void disableUpdate();

        virtual void enableUpdate();

        virtual void arrowCursor();

        /*!
        \brief Groups selected objects
        */
        virtual void createItemGroup();

        /*!
        \brief Method that delete Grouping object selected, but the individual objects continue to exist.
        */
        virtual void destroyItemGroup();

        /*!
        \brief Saves each item in the scene as image. Ex .: .png
        */
        virtual void exportItemsToImage();

        virtual void newTemplate();

        /*!
        \brief Method that exports all the objects in the scene to a template. Ex.: JSON.

        \param type type of template. Ex .: JSON type
        \return true if exported, false otherwise
        */
        virtual bool exportProperties(EnumType* type);

        /*!
        \brief Method that import a template and build all objects. Ex.: JSON.

        \return true if exported, false otherwise
        */
        virtual bool importTemplate(EnumType* type);

        /*!
        \brief Method that instantiates and shows the Page Setup window.
        */
        virtual void showPageSetup();

        virtual void createLineItem();

        virtual void createPolygonItem();

        virtual void createItem(EnumType* itemType);

        virtual ContextObject getContext();

      public slots:
    
    /*!
          \brief Notifies View object that some action on the toolbar has been thrown. 
        */
        virtual void onToolbarChangeMode(te::layout::EnumType* newMode);

    /*!
          \brief Notifies View object that some action on the Menu has been thrown. 
        */
        virtual void onMainMenuChangeMode(te::layout::EnumType* newMode);

    /*!
          \brief Notifies View object that some configuration was modified in the Page Settings Window.
        */
        virtual void onChangeConfig();

        virtual void onSystematicApply(double scale, SystematicScaleType type);

    /*!
          \brief Notifies View object that the selection of objects in the scene changed.
        */
        virtual void onSelectionChanged();
        
        virtual void onSelectionItem(std::string name);

         /*!
          \brief Sets the zoom of the QGraphicsView to fit the given rect
        */
        virtual void fitZoom(const QRectF& rect);

        /*!
          \brief Sets the zoom of the View to the given value
        */
        virtual void setZoom(int zoom);

      signals:

    /*!
          \brief This signal is emitted when the mouse move changed.
      
      \param pos current position
        */
        void changeSceneCoordMouse(QPointF pos);

    /*!
          \brief This signal is emitted when show View object.
        */
        void showView();
  
    /*!
          \brief This signal is emitted when close View object.
        */
        void closeView();

    /*!
          \brief This signal is emitted when hide View object.
        */
        void hideView();
    
    /*!
          \brief This signal is emitted when selection change, mouse release or property refresh requested.
        */
        void reloadProperties(); 

    /*!
          \brief This signal is emitted when View object changes the zoom factor internally.
          */
        void zoomChanged(int zoom);

        /*!
          \brief This signal is emitted when context change.
        */
        void changeContext();

        /*!
          \brief This signal is emitted when View object changes the mode type internally.
          */
        void modeChanged(int mode);

      protected:

    /*!
          \brief Reimplemented from QGraphicsView. Call method createItem() of the scene.
        */
        virtual void mousePressEvent(QMouseEvent * event);

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void  mouseMoveEvent ( QMouseEvent * event );

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void  mouseReleaseEvent ( QMouseEvent * event );

        /*!
        \brief Reimplemented from QGraphicsView
        */
        virtual void	mouseDoubleClickEvent(QMouseEvent * event);

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void wheelEvent(QWheelEvent *event);

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void keyPressEvent(QKeyEvent* keyEvent);

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void resizeEvent(QResizeEvent * event);

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void hideEvent ( QHideEvent * event );

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void closeEvent ( QCloseEvent * event );

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void  showEvent ( QShowEvent * event );

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void  contextMenuEvent ( QContextMenuEvent * event );

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void drawForeground ( QPainter * painter, const QRectF & rect );

        virtual void showSystematicScale();
        
        virtual QCursor createCursor(std::string pathIcon);

        /*!
          \brief Reload properties because selection change
        */
        virtual void reload();

      protected:

        /*!
          \brief Apply the zoom in the QGraphicsView to the given value
          */
        virtual void applyScale(double newScale);

      protected:

        VisualizationArea*            m_visualizationArea;
        AbstractLayoutTool*           m_currentTool;
        PageSetupOutside*             m_pageSetupOutside;
        SystematicScaleOutside*       m_systematicOutside;
        te::gm::Coord2D               m_coordSystematic;
        bool                          m_selectionChange;
        MenuBuilder*                  m_menuBuilder;
        HorizontalRuler*              m_horizontalRuler;
        VerticalRuler*                m_verticalRuler;
        double                        m_width;
        double                        m_height;
        bool                          m_isMoving;
        te::layout::MovingItemGroup*  m_movingItemGroup;
        bool                          m_updateItemPos;
        WaitView*                     m_wait;
        bool                          m_flag;
        QPixmap                       m_foreground; //!< This pixmap represents the foreground drawings and is used for double buffering
    };
  }
}
#endif
