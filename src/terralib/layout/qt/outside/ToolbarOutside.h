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
  \file ToolbarOutside.h
   
  \brief Standard Toolbar for Layout. Includes Qt components for interaction between the user and the plugin/app, via events, made from the Layout module. 
    Ex .: button for create Map, activate ZoomOut function, align selected components to the left, etc. 
    He is also the son of OutsideObserver, so it can become observer of a model (Observable). "View" part of widget.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TOOLBAR_OUTSIDE_H 
#define __TERRALIB_LAYOUT_INTERNAL_TOOLBAR_OUTSIDE_H

// Qt
#include <QToolBar>

// TerraLib
#include "../../core/pattern/mvc/AbstractOutsideView.h"
#include "../../../geometry/Envelope.h"
#include "../../core/Config.h"

// STL
#include <string>

class QGraphicsItem;
class QWidget;
class QGroupBox;
class QAction;
class QToolButton;
class QMenu;
class QPushButton;
class QComboBox;

namespace te
{
  namespace layout
  {
    class EnumType;
    class Scene;
    class AbstractOutsideController;

    /*!
    \brief Standard Toolbar for Layout. Includes Qt components for interaction between the user and the plugin/app, via events, made from the Layout module. 
       Ex .: button for create Map, activate ZoomOut function, align selected components to the left, etc. 
       This is also son of OutsideObserver, so it can become observer of a model (Observable). "View" part of widget.
    
      \ingroup layout

      \sa te::layout::OutsideObserver
    */
    class TELAYOUTEXPORT ToolbarOutside : public QToolBar, public AbstractOutsideView
    {
      Q_OBJECT //for slots/signals

    public:

      ToolbarOutside(AbstractOutsideController* controller);

      virtual ~ToolbarOutside();
      
      virtual void setPosition(const double& x, const double& y);

      virtual te::gm::Coord2D getPosition();

      virtual void createToolbar();

      virtual void notifyChangedZoom(int newZoom);

      virtual void changeAction(EnumType* mode);

      virtual QToolButton* getMapToolButton();

      virtual QToolButton* getMapToolsToolButton();

      virtual QToolButton* getGeometryToolButton();

      virtual QToolButton* getViewAreaToolButton();

      virtual QToolButton* getArrowCursorButton();

      virtual QToolButton* getItemTools();
            
      virtual QToolButton* getBringToFrontToolButton();

      virtual QToolButton* getSendToBackToolButton();

      virtual QToolButton* getRecomposeToolButton();

      virtual QToolButton* getTextToolButton();

      virtual QToolButton* getAlignLeftToolButton();

      virtual QToolButton* getAlignRightToolButton();

      virtual QToolButton* getAlignTopToolButton();

      virtual QToolButton* getAlignBottomToolButton();

      virtual QToolButton* getAlignCenterHorizontalToolButton();

      virtual QToolButton* getAlignCenterVerticalToolButton();

      virtual QToolButton* getRemoveObjectToolButton();

      virtual QToolButton* getUndoToolButton();

      virtual QToolButton* getDrawMapToolButton();

      virtual QToolButton* getObjectToImageButton();

      virtual QToolButton* getExitButton();

      virtual QComboBox* getComboBoxZoom();

      /* Actions (Widget's) */

      virtual QAction* getActionMapToolButton();

      virtual QAction* getActionMapToolsToolButton();

      virtual QAction* getActionGeometryToolButton();

      virtual QAction* getActionViewAreaToolButton();

      virtual QAction* getActionArrowCursorButton();

      virtual QAction* getActionItemTools();
      
      virtual QAction* getActionBringToFrontToolButton();

      virtual QAction* getActionSendToBackToolButton();

      virtual QAction* getActionRecomposeToolButton();

      virtual QAction* getActionTextToolButton();

      virtual QAction* getActionAlignLeftToolButton();

      virtual QAction* getActionAlignRightToolButton();

      virtual QAction* getActionAlignTopToolButton();

      virtual QAction* getActionAlignBottomToolButton();

      virtual QAction* getActionAlignCenterHorizontalToolButton();

      virtual QAction* getActionAlignCenterVerticalToolButton();

      virtual QAction* getActionRemoveObjectToolButton();

      virtual QAction* getActionUndoToolButton();

      virtual QAction* getActionDrawMapToolButton();

      virtual QAction* getActionObjectToImageButton();

      virtual QAction* getActionExitButton();

      virtual QAction* getActionComboBoxZoom();

      /* Actions names */

      /* Map Menu */

      std::string getActionMapDefault();

      std::string getActionLegendDefault();

      std::string getActionScale();

      /* Map Tools */

      std::string getActionMapZoomIn();

      std::string getActionMapZoomOut();

      std::string getActionMapPan();

      std::string getActionMapSystematicScale();

      std::string getActionMapCreateTextGrid();

      std::string getActionMapCreateMapText();

      std::string getActionMapCreateLegendChildAsObject();
      
      std::string getActionGridPlanar();

      std::string getActionGridGeodesic();

      std::string getActionNorth();

      std::string getActionMapLocation();

      /* Geometry Menu */

      std::string getActionRectangle();

      std::string getActionPoint();

      std::string getActionEllipse();

      std::string getActionArrow();

      std::string getActionLine(); 

      std::string getActionPolygon();

      std::string getActionSVG();

      /* View Area */

      std::string getActionViewPan();

      std::string getActionViewZoomIn();

      std::string getActionViewZoomOut();

      /* Items Tools */

      std::string getActionGroup();

      std::string getActionUngroup();
      
      std::string getActionSceneZoom();

      std::string getActionRemoveObject();

      /* Text Tools */

      std::string getActionTextDefault();

      std::string getActionImage();

      std::string getActionStringGrid();

      std::string getActionTitle();

      std::string getActionBalloon();

      std::string getActionBarCode();


      /* Align */

      std::string getActionAlignLeft();

      std::string getActionAlignRight();

      std::string getActionAlignTop();

      std::string getActionAlignBottom();

      std::string getActionAlignCenterHorizontal();

      std::string getActionAlignCenterVertical();

      /* Undo/Redo */

      std::string getActionUndo();

      std::string getActionRedo();

      std::string getActionDrawMap();

      std::string getActionObjectToImage();

      std::string getActionExit();

      std::string getActionExportToPDF();
                  
    signals:
      
      void changeMode(te::layout::EnumType* newMode);

      void zoomChangedInComboBox(int zoom);

    protected:

      virtual QToolButton* createMapToolButton();

      virtual QToolButton* createMapToolsToolButton();

      virtual QToolButton* createGeometryToolButton();

      virtual QToolButton* createViewAreaToolButton();

      virtual QToolButton* createArrowCursorButton();

      virtual QToolButton* createItemTools();
            
      virtual QToolButton* createBringToFrontToolButton();

      virtual QComboBox* createSceneZoomCombobox();

      virtual QToolButton* createSendToBackToolButton();

      virtual QToolButton* createRecomposeToolButton();

      virtual QToolButton* createTextToolButton();

      virtual QToolButton* createAlignLeftToolButton();

      virtual QToolButton* createAlignRightToolButton();

      virtual QToolButton* createAlignTopToolButton();

      virtual QToolButton* createAlignBottomToolButton();

      virtual QToolButton* createAlignCenterHorizontalToolButton();

      virtual QToolButton* createAlignCenterVerticalToolButton();

      virtual QToolButton* createRemoveObjectToolButton();

      virtual QToolButton* createUndoToolButton();

      virtual QToolButton* createDrawMapToolButton();

      virtual QToolButton* createObjectToImageButton();

      virtual QToolButton* createExitButton();

      virtual QToolButton* createExportToPDFButton();
      
      virtual QToolButton* createToolButton(std::string text, std::string tooltip, std::string icon);

      virtual QPushButton* createPushButton(std::string text, std::string tooltip, std::string icon);
      
      virtual QAction* createAction(std::string text, std::string objName, std::string icon, std::string tooltip = "", QWidget* parent = 0);

      Scene* getScene();

    protected:

      QComboBox* m_comboZoom;

      /* Map Menu */
      std::string m_actionMapDefault;
      std::string m_actionLegendDefault;
      std::string m_actionScale;

      /* Map Tools */
      std::string m_actionMapZoomIn;
      std::string m_actionMapZoomOut;
      std::string m_actionMapPan;
      std::string m_actionMapSystematicScale;
      std::string m_actionMapCreateTextGrid;
      std::string m_actionMapCreateMapText;
      std::string m_actionMapCreateLegendChildAsObject;
      std::string m_actionGridPlanar;
      std::string m_actionGridGeodesic;
      std::string m_actionNorth;
      std::string m_actionMapLocation;
      
      /* Geometry Menu */
      std::string m_actionRectangle;
      std::string m_actionPoint;
      std::string m_actionEllipse;
      std::string m_actionArrow;
      std::string m_actionLine; 
      std::string m_actionPolygon;
      std::string m_actionSVG;

      /* View Area */
      std::string m_actionViewPan;
      std::string m_actionViewZoomIn;
      std::string m_actionViewZoomOut;
      
      /* Items Tools */

      std::string m_actionGroup;
      std::string m_actionUngroup;
      
      std::string m_actionSceneZoom;

      std::string m_actionRemoveObject;

      /* Text Tools */
      std::string m_actionTextDefault;
      std::string m_actionImage;
      std::string m_actionStringGrid;
      std::string m_actionTitle;
      std::string m_actionBalloon;
      std::string m_actionBarCode;

      /* Align */
      std::string m_actionAlignLeft;
      std::string m_actionAlignRight;
      std::string m_actionAlignTop;
      std::string m_actionAlignBottom;
      std::string m_actionAlignCenterHorizontal;
      std::string m_actionAlignCenterVertical;

      /* Undo/Redo */
      std::string m_actionUndo;
      std::string m_actionRedo;

      std::string m_actionDrawMap;

      std::string m_actionObjectToImage;
      std::string m_actionExit;
      std::string m_actionExportToPDF;

      /* ToolButton */
      QToolButton* m_mapToolButton;
      QToolButton* m_mapToolsToolButton;
      QToolButton* m_geometryToolButton;
      QToolButton* m_viewAreaToolButton;
      QToolButton* m_arrowCursorButton;
      QToolButton* m_itemTools;
      QToolButton* m_bringToFrontToolButton;
      QToolButton* m_sendToBackToolButton;
      QToolButton* m_recomposeToolButton;
      QToolButton* m_textToolButton;
      QToolButton* m_alignLeftToolButton;
      QToolButton* m_alignRightToolButton;
      QToolButton* m_alignTopToolButton;
      QToolButton* m_alignBottomToolButton;
      QToolButton* m_alignCenterHorizontalToolButton;
      QToolButton* m_alignCenterVerticalToolButton;
      QToolButton* m_removeObjectToolButton;
      QToolButton* m_undoToolButton;
      QToolButton* m_drawMapToolButton;
      QToolButton* m_objectToImageButton;
      QToolButton* m_exitButton;
      QToolButton* m_exportToPDFButton;

      /* Actions (Widget's) */

      QAction* m_actionComboZoom;
      QAction* m_actionMapToolButton;
      QAction* m_actionMapToolsToolButton;
      QAction* m_actionGeometryToolButton;
      QAction* m_actionViewAreaToolButton;
      QAction* m_actionArrowCursorButton;
      QAction* m_actionItemTools;
      QAction* m_actionBringToFrontToolButton;
      QAction* m_actionSendToBackToolButton;
      QAction* m_actionRecomposeToolButton;
      QAction* m_actionTextToolButton;
      QAction* m_actionAlignLeftToolButton;
      QAction* m_actionAlignRightToolButton;
      QAction* m_actionAlignTopToolButton;
      QAction* m_actionAlignBottomToolButton;
      QAction* m_actionAlignCenterHorizontalToolButton;
      QAction* m_actionAlignCenterVerticalToolButton;
      QAction* m_actionRemoveObjectToolButton;
      QAction* m_actionUndoToolButton;
      QAction* m_actionDrawMapToolButton;
      QAction* m_actionObjectToImageButton;
      QAction* m_actionExitButton;
      QAction* m_actionExportToPDFButton;
    };
  }
}

#endif
