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
          \brief Returns value that represents create scale mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateScale() const;
								
        /*!
          \brief Returns value that represents printer mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModePrinter() const;

        /*!
          \brief Returns value that represents create image mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateImage() const;
				
        /*!
          \brief Returns value that represents systematic scale mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeSystematicScale() const;

        /*!
          \brief Returns value that represents svgview mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeSVGView() const;

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
          \brief Returns value that represents object inspector current item changed type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeInspectorCurrentItemChanged() const;
				
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

        /*!
          \brief Returns value that represents create north mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateNorth() const;

        /*!
          \brief Returns value that represents create map location mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateMapLocation() const;

        /*!
          \brief Returns value that represents create map location mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeCreateSVG() const;

        /*!
          \brief Returns value that represents text editor mode type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getModeTextEditorInteraction() const;

				/*!
				\brief Returns value that represents arrow mode type belonging to enumeration.

				\return enum value
				*/
				virtual EnumType* getModeArrowCursor() const;

      protected:

        /*!
          \brief Reimplemented from AbstractEnum
        */
        virtual void init();

      protected:
        
        EnumType* m_modeNone; //!< value that represents none mode type belonging to enumeration
        EnumType* m_modeMapPan; //!< value that represents map pan mode type belonging to enumeration
        EnumType* m_modeMapZoomIn; //!< value that represents map zoom in mode type belonging to enumeration
        EnumType* m_modeMapZoomOut; //!< value that represents map zoom out mode type belonging to enumeration
        EnumType* m_modeCreateMap; //!< value that represents create map mode type belonging to enumeration
        EnumType* m_modeCreateLegend; //!< value that represents create legend mode type belonging to enumeration
        EnumType* m_modeCreateRectangle; //!< value that represents create rectangle mode type belonging to enumeration
        EnumType* m_modeCreateText; //!< value that represents create text mode type belonging to enumeration
        EnumType* m_modeCreateItemGroup; //!< value that represents create item group mode type belonging to enumeration
        EnumType* m_modeCreateScale; //!< value that represents create scale mode type belonging to enumeration
        EnumType* m_modePrinter; //!< value that represents printer mode type belonging to enumeration
        EnumType* m_modeCreateImage; //!< value that represents create image mode type belonging to enumeration
        EnumType* m_modeSystematicScale; //!< value that represents systematic scale mode type belonging to enumeration
        EnumType* m_modeSVGView; //!< value that represents svgview mode type belonging to enumeration
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
        EnumType* m_modeUndo; //!< value that represents undo mode type belonging to enumeration
        EnumType* m_modeRedo; //!< value that represents redo mode type belonging to enumeration
        EnumType* m_modeCreateLegendChild; //!< value that represents create legend child mode type belonging to enumeration
        EnumType* m_modeLegendChildAsObject; //!< value that represents legend child as object (vectorize) mode type belonging to enumeration
        EnumType* m_inspectorCurrentItemChanged;//!< value that represents object inspector current item changed type belonging to enumeration
        EnumType* m_modeCreateLine; //!< value that represents create line mode type belonging to enumeration
        EnumType* m_modeCreatePolygon; //!< value that represents create polygon mode type belonging to enumeration
        EnumType* m_modeCreateBalloon; //!< value that represents create balloon mode type belonging to enumeration
        EnumType* m_modeCreateBarCode; //!< value that represents create barcode mode type belonging to enumeration
        EnumType* m_modeCreateGridMap; //!< value that represents create gridmap mode type belonging to enumeration
        EnumType* m_coordWait; //!< value that represents wait coordinate mode type belonging to enumeration
        EnumType* m_modeCreateGridPlanar; //!< value that represents create grid planar mode type belonging to enumeration
        EnumType* m_modeCreateGridGeodesic; //!< value that represents create grid geodesic mode type belonging to enumeration
        EnumType* m_modeCreateNorth; //!< value that represents create north mode type belonging to enumeration
        EnumType* m_modeCreateMapLocation; //!< value that represents create map location mode type belonging to enumeration
        EnumType* m_modeCreateSVG; //!< value that represents create SVG type belonging to enumeration
        EnumType* m_modeTextEditorInteraction; //!< value that represents text editor type belonging to enumeration
				EnumType* m_modeArrowCursor; //!< value that represents arrow mode type belonging to enumeration
    };
  }
}

#endif

