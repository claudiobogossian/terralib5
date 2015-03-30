/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
#include "../../core/pattern/mvc/OutsideObserver.h"
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

    /*!
    \brief Standard Toolbar for Layout. Includes Qt components for interaction between the user and the plugin/app, via events, made from the Layout module. 
       Ex .: button for create Map, activate ZoomOut function, align selected components to the left, etc. 
       This is also son of OutsideObserver, so it can become observer of a model (Observable). "View" part of widget.
	  
	    \ingroup layout

	    \sa te::layout::OutsideObserver
	  */
    class TELAYOUTEXPORT ToolbarOutside : public QToolBar, public OutsideObserver
    {
	    Q_OBJECT //for slots/signals

    public:

	    ToolbarOutside(OutsideController* controller, Observable* o);

	    virtual ~ToolbarOutside();

	    virtual void updateObserver(ContextItem context);

	    virtual void setPosition(const double& x, const double& y);

	    virtual te::gm::Coord2D getPosition();

      virtual void createToolbar();

      virtual QToolButton* getMapToolButton();

      virtual QToolButton* getMapToolsToolButton();

      virtual QToolButton* getGeometryToolButton();

      virtual QToolButton* getViewAreaToolButton();

      virtual QToolButton* getArrowCursorButton();

      virtual QToolButton* getItemTools();

      virtual QToolButton* getLineIntersectionToolButton();
      
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
      
    public slots:
      
      virtual void onMapTriggered(QAction* action);

      virtual void onMapToolsTriggered(QAction* action);

      virtual void onGeometryTriggered(QAction* action);

      virtual void onViewAreaTriggered(QAction* action);
      
      virtual void onArrowCursorClicked(bool checked);

      virtual void onItemToolsTriggered(QAction* action);

      virtual void onLineIntersectionMouse(bool checked);

      virtual void onSceneZoomCurrentIndexChanged(int index);

      virtual void onBringToFrontClicked(bool checked);

      virtual void onSendToBackClicked(bool checked);

      virtual void onRecomposeClicked(bool checked);

      virtual void onTextToolsTriggered(QAction* action);

      virtual void onAlignLeftClicked(bool checked);

      virtual void onAlignRightClicked(bool checked);

      virtual void onAlignTopClicked(bool checked);

      virtual void onAlignBottomClicked(bool checked);

      virtual void onAlignCenterHorizontalClicked(bool checked);

      virtual void onAlignCenterVerticalClicked(bool checked);

      virtual void onRemoveObjectClicked(bool checked);

      virtual void onDrawMapClicked(bool checked);

      virtual void onObjectToImageClicked(bool checked);

      virtual void onExitClicked(bool checked);

      virtual void onExportToPDFClicked(bool checked);
      
    signals:

      void changeContext(bool change);

    protected slots:

      virtual void onChangeZoom(double factor);
      
    protected:

      virtual QToolButton* createMapToolButton();

      virtual QToolButton* createMapToolsToolButton();

      virtual QToolButton* createGeometryToolButton();

      virtual QToolButton* createViewAreaToolButton();

      virtual QToolButton* createArrowCursorButton();

      virtual QToolButton* createItemTools();

      virtual QToolButton* createLineIntersectionToolButton();
      
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

      virtual void changeAction(EnumType* mode);

      virtual QToolButton* createToolButton(std::string text, std::string tooltip, std::string icon);

      virtual QPushButton* createPushButton(std::string text, std::string tooltip, std::string icon);
      
      virtual QAction* createAction(std::string text, std::string objName, std::string icon, std::string tooltip = "", QWidget* parent = 0);

    protected:

      QComboBox* m_comboZoom;

      /* Map Menu */
      std::string m_optionMapDefault;
      std::string m_optionMapGrid;
      std::string m_optionLegendDefault;
      std::string m_optionScale;
      std::string m_optionGridMap;

      /* Map Tools */
      std::string m_optionMapZoomIn;
      std::string m_optionMapZoomOut;
      std::string m_optionMapPan;
      std::string m_optionMapSystematicScale;
      std::string m_optionMapCreateTextGrid;
      std::string m_optionMapCreateMapText;
      std::string m_optionMapCreateLegendChildAsObject;
      
      /* Geometry Menu */
      std::string m_optionRectangle;
      std::string m_optionPoint;
      std::string m_optionEllipse;
      std::string m_optionArrow;
      std::string m_optionLine; 
      std::string m_optionPolygon; 

      /* View Area */
      std::string m_optionViewPan;
      std::string m_optionViewZoomIn;
      std::string m_optionViewZoomOut;
      
      /* Items Tools */

      std::string m_optionGroup;
      std::string m_optionUngroup;

      std::string m_optionLineIntersectionMouse;

      std::string m_optionSceneZoom;

      std::string m_optionRemoveObject;

      /* Text Tools */
      std::string m_optionTextDefault;
      std::string m_optionImage;
      std::string m_optionStringGrid;
      std::string m_optionTitle;
      std::string m_optionBalloon;
      std::string m_optionBarCode;

      /* Align */
      std::string m_optionAlignLeft;
      std::string m_optionAlignRight;
      std::string m_optionAlignTop;
      std::string m_optionAlignBottom;
      std::string m_optionAlignCenterHorizontal;
      std::string m_optionAlignCenterVertical;

      /* Undo/Redo */
      std::string m_optionUndo;
      std::string m_optionRedo;

      std::string m_optionDrawMap;

      std::string m_optionObjectToImage;
      std::string m_optionExit;
      std::string m_optionExportToPDF;

      /* ToolButton */
      QToolButton* m_mapToolButton;
      QToolButton* m_mapToolsToolButton;
      QToolButton* m_geometryToolButton;
      QToolButton* m_viewAreaToolButton;
      QToolButton* m_arrowCursorButton;
      QToolButton* m_itemTools;
      QToolButton* m_lineIntersectionToolButton;
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
    };
  }
}

#endif
