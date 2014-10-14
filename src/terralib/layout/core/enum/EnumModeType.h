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
  \file EnumModeType.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ENUM_MODESTYLE_TYPE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ENUM_MODESTYLE_TYPE_H

// TerraLib
#include "AbstractEnum.h"
#include "../Config.h"

namespace te
{
  namespace layout
  {
    class EnumType;

    class TELAYOUTEXPORT EnumModeType : public AbstractEnum
    {
      public:

        EnumModeType(); 

        virtual ~EnumModeType();
        
        virtual EnumType* getModeSelectByBox() const;
        virtual EnumType* getModeSelect() const;
        virtual EnumType* getModeArrowCursor() const;
        virtual EnumType* getModePan() const;
        virtual EnumType* getModeZoomIn() const;
        virtual EnumType* getModeZoomOut() const;
        virtual EnumType* getModeMapPan() const;
        virtual EnumType* getModeMapZoomIn() const;
        virtual EnumType* getModeMapZoomOut() const;
        virtual EnumType* getModeCreateMap() const;
        virtual EnumType* getModeCreateMapGrid() const;
        virtual EnumType* getModeCreateLegend() const;
        virtual EnumType* getModeCreateRectangle() const;
        virtual EnumType* getModeCreateText() const;
        virtual EnumType* getModeCreateItemGroup() const;
        virtual EnumType* getModeUnitsMetricsChange() const;
        virtual EnumType* getModeExportPropsJSON() const;
        virtual EnumType* getModeImportJSONProps() const;
        virtual EnumType* getModeNewTemplate() const;
        virtual EnumType* getModeCreateScale() const;
        virtual EnumType* getModeGroup() const;
        virtual EnumType* getModeUngroup() const;
        virtual EnumType* getModeActiveLinesIntersectionMouse() const;
        virtual EnumType* getModeOffLinesIntersectionMouse() const;
        virtual EnumType* getModePrinter() const;
        virtual EnumType* getModeExit() const;
        virtual EnumType* getModeSceneZoom() const;
        virtual EnumType* getModeBringToFront() const;
        virtual EnumType* getModeSendToBack() const;
        virtual EnumType* getModeRecompose() const;
        virtual EnumType* getModeCreateImage() const;
        virtual EnumType* getModePageConfig() const;
        virtual EnumType* getModeSystematicScale() const;
        virtual EnumType* getModeCreatePoint() const;
        virtual EnumType* getModeCreateArrow() const;
        virtual EnumType* getModeCreateEllipse() const;
        virtual EnumType* getModeCreateTitle() const;
        virtual EnumType* getModeCreateTextGrid() const;
        virtual EnumType* getModeCreateMultiLineText() const;
        virtual EnumType* getModeWidgetView() const;
        virtual EnumType* getModeWidgetInspector() const;
        virtual EnumType* getModeWidgetProperties() const;
        virtual EnumType* getModeWidgetToolbar() const;
        virtual EnumType* getModeMapCreateTextGrid() const;
        virtual EnumType* getModeMapCreateTextMap() const;
        virtual EnumType* getModeAlignCenterHorizontal() const;
        virtual EnumType* getModeAlignCenterVertical() const;
        virtual EnumType* getModeAlignLeft() const;
        virtual EnumType* getModeAlignRight() const;
        virtual EnumType* getModeAlignTop() const;
        virtual EnumType* getModeAlignBottom() const;
        virtual EnumType* getModeRemoveObject() const;
        virtual EnumType* getModeNone() const;
        virtual EnumType* getModeUndo() const;
        virtual EnumType* getModeRedo() const;
        virtual EnumType* getModeDrawSelectionMap() const;
        virtual EnumType* getModeCreateLegendChild() const;
        virtual EnumType* getModeLegendChildAsObject() const;

      protected:

        virtual void init();

      protected:
        
        EnumType* m_modeNone;
        EnumType* m_modeSelectByBox;
        EnumType* m_modeSelect;
        EnumType* m_modeArrowCursor;
        EnumType* m_modePan;
        EnumType* m_modeZoomIn;
        EnumType* m_modeZoomOut;
        EnumType* m_modeMapPan;
        EnumType* m_modeMapZoomIn;
        EnumType* m_modeMapZoomOut;
        EnumType* m_modeCreateMap;
        EnumType* m_modeCreateMapGrid;
        EnumType* m_modeCreateLegend;
        EnumType* m_modeCreateRectangle;
        EnumType* m_modeCreateText;
        EnumType* m_modeCreateItemGroup;
        EnumType* m_modeUnitsMetricsChange;
        EnumType* m_modeExportPropsJSON;
        EnumType* m_modeImportJSONProps;
        EnumType* m_modeNewTemplate;
        EnumType* m_modeCreateScale;
        EnumType* m_modeGroup;
        EnumType* m_modeUngroup;
        EnumType* m_modeActiveLinesIntersectionMouse;
        EnumType* m_modeOffLinesIntersectionMouse;
        EnumType* m_modePrinter;
        EnumType* m_modeExit;
        EnumType* m_modeSceneZoom;
        EnumType* m_modeBringToFront;
        EnumType* m_modeSendToBack;
        EnumType* m_modeRecompose;
        EnumType* m_modeCreateImage;
        EnumType* m_modePageConfig;
        EnumType* m_modeSystematicScale;
        EnumType* m_modeCreatePoint;
        EnumType* m_modeCreateArrow;
        EnumType* m_modeCreateEllipse;
        EnumType* m_modeCreateTitle;
        EnumType* m_modeCreateTextGrid;
        EnumType* m_modeCreateMultiLineText;
        EnumType* m_modeWidgetView;
        EnumType* m_modeWidgetInspector;
        EnumType* m_modeWidgetProperties;
        EnumType* m_modeWidgetToolbar;
        EnumType* m_modeMapCreateTextGrid;
        EnumType* m_modeMapCreateTextMap;
        EnumType* m_modeAlignCenterHorizontal;
        EnumType* m_modeAlignCenterVertical;
        EnumType* m_modeAlignLeft;
        EnumType* m_modeAlignRight;
        EnumType* m_modeAlignTop;
        EnumType* m_modeAlignBottom;
        EnumType* m_modeRemoveObject;
        EnumType* m_modeUndo;
        EnumType* m_modeRedo;
        EnumType* m_modeDrawSelectionMap;
        EnumType* m_modeCreateLegendChild;
        EnumType* m_modeLegendChildAsObject;
    };
  }
}

#endif
