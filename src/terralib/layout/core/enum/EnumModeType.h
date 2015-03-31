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
   
  \brief Class to represent a mode type enumeration. Ex.: select, pan, create text, etc.
    The type of mode is used by the context to know what should be done. The mode in context could be modified by the user interaction.

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

    /*!
    \brief Class to represent a mode type enumeration. Ex.: select, pan, create text, etc.
      The type of mode is used by the context to know what should be done. The mode in context could be modified by the user interaction.
	  
	    \ingroup layout

	    \sa te::layout::AbstractEnum
	  */
    class TELAYOUTEXPORT EnumModeType : public AbstractEnum
    {
      public:

        /*!
          \brief Constructor
        */
        EnumModeType(); 

        /*!
          \brief Destructor
        */ 
        virtual ~EnumModeType();
        
        /*!
          \brief Returns value that represents none mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeNone() const;

        /*!
          \brief Returns value that represents select by box mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeSelectByBox() const;

        /*!
          \brief Returns value that represents select mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeSelect() const;

        /*!
          \brief Returns value that represents arrow mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeArrowCursor() const;
        
        /*!
          \brief Returns value that represents pan mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModePan() const;

        /*!
          \brief Returns value that represents zoom in mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeZoomIn() const;

        /*!
          \brief Returns value that represents zoom out mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeZoomOut() const;

        /*!
          \brief Returns value that represents map pan mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeMapPan() const;
        
        /*!
          \brief Returns value that represents map zoom in mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeMapZoomIn() const;
        
        /*!
          \brief Returns value that represents map zoom out mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeMapZoomOut() const;

        /*!
          \brief Returns value that represents create map mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateMap() const;

        /*!
          \brief Returns value that represents create map grid mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateMapGrid() const;

        /*!
          \brief Returns value that represents create legend mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateLegend() const;

        /*!
          \brief Returns value that represents create rectangle mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateRectangle() const;

        /*!
          \brief Returns value that represents create text mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateText() const;

        /*!
          \brief Returns value that represents create item group mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateItemGroup() const;

        /*!
          \brief Returns value that represents units metrics change mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeUnitsMetricsChange() const;

        /*!
          \brief Returns value that represents export properties to json mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeExportPropsJSON() const;

        /*!
          \brief Returns value that represents import properties from json mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeImportJSONProps() const;

        /*!
          \brief Returns value that represents new template mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeNewTemplate() const;

        /*!
          \brief Returns value that represents create scale mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateScale() const;

        /*!
          \brief Returns value that represents group mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeGroup() const;

        /*!
          \brief Returns value that represents ungroup mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeUngroup() const;

        /*!
          \brief Returns value that represents active lines intersection mouse mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeActiveLinesIntersectionMouse() const;

        /*!
          \brief Returns value that represents off lines intersection mouse mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeOffLinesIntersectionMouse() const;

        /*!
          \brief Returns value that represents printer mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModePrinter() const;

        /*!
          \brief Returns value that represents exit mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeExit() const;

        /*!
          \brief Returns value that represents scene zoom mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeSceneZoom() const;

        /*!
          \brief Returns value that represents bring to front mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeBringToFront() const;

        /*!
          \brief Returns value that represents send to back mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeSendToBack() const;

        /*!
          \brief Returns value that represents recompose mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeRecompose() const;

        /*!
          \brief Returns value that represents create image mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateImage() const;

        /*!
          \brief Returns value that represents page configuration mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModePageConfig() const;

        /*!
          \brief Returns value that represents systematic scale mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeSystematicScale() const;

        /*!
          \brief Returns value that represents create point mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreatePoint() const;

        /*!
          \brief Returns value that represents create arrow mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateArrow() const;

        /*!
          \brief Returns value that represents create ellipse mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateEllipse() const;

        /*!
          \brief Returns value that represents create title mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateTitle() const;

        /*!
          \brief Returns value that represents create text grid mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateTextGrid() const;

        /*!
          \brief Returns value that represents QGraphicsView widget mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeWidgetView() const;

        /*!
          \brief Returns value that represents Object Inspector widget mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeWidgetInspector() const;

        /*!
          \brief Returns value that represents Properties widget mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeWidgetProperties() const;

        /*!
          \brief Returns value that represents Toolbar widget mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeWidgetToolbar() const;

        /*!
          \brief Returns value that represents create text grid (vectorize) mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeMapCreateTextGrid() const;

        /*!
          \brief Returns value that represents create text map (vectorize) mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeMapCreateTextMap() const;

        /*!
          \brief Returns value that represents align center horizontal mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeAlignCenterHorizontal() const;

        /*!
          \brief Returns value that represents align center vertical mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeAlignCenterVertical() const;

        /*!
          \brief Returns value that represents align left mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeAlignLeft() const;

        /*!
          \brief Returns value that represents align right mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeAlignRight() const;

        /*!
          \brief Returns value that represents align top mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeAlignTop() const;

        /*!
          \brief Returns value that represents align bottom mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeAlignBottom() const;

        /*!
          \brief Returns value that represents remove object (MVC component) mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeRemoveObject() const;

        /*!
          \brief Returns value that represents undo mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeUndo() const;

        /*!
          \brief Returns value that represents redo mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeRedo() const;

        /*!
          \brief Returns value that represents draw selection map (MapDisplay proxy) mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeDrawSelectionMap() const;

        /*!
          \brief Returns value that represents create legend child mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateLegendChild() const;

        /*!
          \brief Returns value that represents legend child as object (vectorize) mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeLegendChildAsObject() const;

        /*!
          \brief Returns value that represents object to image (MVC Component to image. Ex.: .png) mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeObjectToImage() const;

        /*!
          \brief Returns value that represents object inspector current item changed type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeInspectorCurrentItemChanged() const;

        /*!
          \brief Returns value that represents export to PDF mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeExportToPDF() const;

        /*!
          \brief Returns value that represents create line mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateLine() const; 

        /*!
          \brief Returns value that represents create polygon mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreatePolygon() const; 

        /*!
          \brief Returns value that represents create balloon mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateBalloon() const; 

        /*!
          \brief Returns value that represents wait coordinate mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateBarCode() const; 

        /*!
          \brief Returns value that represents wait coordinate mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateGridMap() const;

        /*!
          \brief Returns value that represents wait coordinate mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCoordWait() const;

        /*!
          \brief Returns value that represents create grid planar mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateGridPlanar() const;

        /*!
          \brief Returns value that represents create grid geodesic mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateGridGeodesic() const;

      protected:

        /*!
          \brief Reimplemented from AbstractEnum
        */
        virtual void init();

      protected:
        
        EnumType* m_modeNone; //!< value that represents none mode type belonging to enumeration
        EnumType* m_modeSelectByBox; //!< value that represents select by box mode type belonging to enumeration
        EnumType* m_modeSelect; //!< value that represents select mode type belonging to enumeration
        EnumType* m_modeArrowCursor; //!< value that represents arrow mode type belonging to enumeration
        EnumType* m_modePan; //!< value that represents pan mode type belonging to enumeration
        EnumType* m_modeZoomIn; //!< value that represents zoom in mode type belonging to enumeration
        EnumType* m_modeZoomOut; //!< value that represents zoom out mode type belonging to enumeration
        EnumType* m_modeMapPan; //!< value that represents map pan mode type belonging to enumeration
        EnumType* m_modeMapZoomIn; //!< value that represents map zoom in mode type belonging to enumeration
        EnumType* m_modeMapZoomOut; //!< value that represents map zoom out mode type belonging to enumeration
        EnumType* m_modeCreateMap; //!< value that represents create map mode type belonging to enumeration
        EnumType* m_modeCreateMapGrid; //!< value that represents create map grid mode type belonging to enumeration
        EnumType* m_modeCreateLegend; //!< value that represents create legend mode type belonging to enumeration
        EnumType* m_modeCreateRectangle; //!< value that represents create rectangle mode type belonging to enumeration
        EnumType* m_modeCreateText; //!< value that represents create text mode type belonging to enumeration
        EnumType* m_modeCreateItemGroup; //!< value that represents create item group mode type belonging to enumeration
        EnumType* m_modeUnitsMetricsChange; //!< value that represents units metrics change mode type belonging to enumeration
        EnumType* m_modeExportPropsJSON; //!< value that represents export properties to json mode type belonging to enumeration
        EnumType* m_modeImportJSONProps; //!< value that represents import properties from json mode type belonging to enumeration
        EnumType* m_modeNewTemplate; //!< value that represents new template mode type belonging to enumeration
        EnumType* m_modeCreateScale; //!< value that represents create scale mode type belonging to enumeration
        EnumType* m_modeGroup; //!< value that represents group mode type belonging to enumeration
        EnumType* m_modeUngroup; //!< value that represents ungroup mode type belonging to enumeration
        EnumType* m_modeActiveLinesIntersectionMouse; //!< value that represents active lines intersection mouse mode type belonging to enumeration
        EnumType* m_modeOffLinesIntersectionMouse; //!< value that represents off lines intersection mouse mode type belonging to enumeration
        EnumType* m_modePrinter; //!< value that represents printer mode type belonging to enumeration
        EnumType* m_modeExit; //!< value that represents exit mode type belonging to enumeration
        EnumType* m_modeSceneZoom; //!< value that represents scene zoom mode type belonging to enumeration
        EnumType* m_modeBringToFront; //!< value that represents bring to front mode type belonging to enumeration
        EnumType* m_modeSendToBack; //!< value that represents send to back mode type belonging to enumeration
        EnumType* m_modeRecompose; //!< value that represents recompose mode type belonging to enumeration
        EnumType* m_modeCreateImage; //!< value that represents create image mode type belonging to enumeration
        EnumType* m_modePageConfig; //!< value that represents page configuration mode type belonging to enumeration
        EnumType* m_modeSystematicScale; //!< value that represents systematic scale mode type belonging to enumeration
        EnumType* m_modeCreatePoint; //!< value that represents create point mode type belonging to enumeration
        EnumType* m_modeCreateArrow; //!< value that represents create arrow mode type belonging to enumeration
        EnumType* m_modeCreateEllipse; //!< value that represents create ellipse mode type belonging to enumeration
        EnumType* m_modeCreateTitle; //!< value that represents create title mode type belonging to enumeration
        EnumType* m_modeCreateTextGrid; //!< value that represents create text grid mode type belonging to enumeration
        EnumType* m_modeWidgetView; //!< value that represents QGraphicsView widget mode type belonging to enumeration
        EnumType* m_modeWidgetInspector; //!< value that represents Object Inspector widget mode type belonging to enumeration
        EnumType* m_modeWidgetProperties; //!< value that represents Properties widget mode type belonging to enumeration
        EnumType* m_modeWidgetToolbar; //!< value that represents Toolbar widget mode type belonging to enumeration
        EnumType* m_modeMapCreateTextGrid; //!< value that represents create text grid (vectorize) mode type belonging to enumeration
        EnumType* m_modeMapCreateTextMap; //!< value that represents create text map (vectorize) mode type belonging to enumeration
        EnumType* m_modeAlignCenterHorizontal; //!< value that represents align center horizontal mode type belonging to enumeration
        EnumType* m_modeAlignCenterVertical; //!< value that represents align center vertical mode type belonging to enumeration
        EnumType* m_modeAlignLeft; //!< value that represents align left mode type belonging to enumeration
        EnumType* m_modeAlignRight; //!< value that represents align right mode type belonging to enumeration
        EnumType* m_modeAlignTop; //!< value that represents align top mode type belonging to enumeration
        EnumType* m_modeAlignBottom; //!< value that represents align bottom mode type belonging to enumeration
        EnumType* m_modeRemoveObject; //!< value that represents remove object (MVC component) mode type belonging to enumeration
        EnumType* m_modeUndo; //!< value that represents undo mode type belonging to enumeration
        EnumType* m_modeRedo; //!< value that represents redo mode type belonging to enumeration
        EnumType* m_modeDrawSelectionMap; //!< value that represents draw selection map (MapDisplay proxy) mode type belonging to enumeration
        EnumType* m_modeCreateLegendChild; //!< value that represents create legend child mode type belonging to enumeration
        EnumType* m_modeLegendChildAsObject; //!< value that represents legend child as object (vectorize) mode type belonging to enumeration
        EnumType* m_modelObjectToImage; //!< value that represents object to image (MVC Component to image. Ex.: .png) mode type belonging to enumeration
        EnumType* m_inspectorCurrentItemChanged;//!< value that represents object inspector current item changed type belonging to enumeration
        EnumType* m_modelExportToPDF; //!< value that represents export to PDF mode type belonging to enumeration
        EnumType* m_modeCreateLine; //!< value that represents create line mode type belonging to enumeration
        EnumType* m_modeCreatePolygon; //!< value that represents create polygon mode type belonging to enumeration
        EnumType* m_modeCreateBalloon; //!< value that represents create balloon mode type belonging to enumeration
        EnumType* m_modeCreateBarCode; //!< value that represents create barcode mode type belonging to enumeration
        EnumType* m_modeCreateGridMap; //!< value that represents create gridmap mode type belonging to enumeration
        EnumType* m_coordWait; //!< value that represents wait coordinate mode type belonging to enumeration
        EnumType* m_modeCreateGridPlanar; //!< value that represents create grid planar mode type belonging to enumeration
        EnumType* m_modeCreateGridGeodesic; //!< value that represents create grid geodesic mode type belonging to enumeration
    };
  }
}

#endif
