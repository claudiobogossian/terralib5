/******************************************************************************
* Funcate - GIS development team
*
* TerraPrint - 
* Copyright © 2013	INPE/Funcate
*
* @file TdkAbstractComponentType.h
* @brief 
* @version 1.0
* @author: Juliana Hohara de Souza Coelho <juliana.coelho@funcate.org.br>
* @date:  
******************************************************************************/

#ifndef LAYOUT_MODE_H
#define LAYOUT_MODE_H

namespace te
{
  namespace layout
  {
    /** 
    * Enum LayoutMode.
    * This is the enumeration of the components types
    * @ingroup factory
    **/
    typedef enum
    {
      TypeSelectByBox,
      TypeSelect,
      TypePan,
      TypeZoomIn,
      TypeZoomOut,
      TypeCreate,
      TypeNone
    } LayoutMode;

    /** 
    * Enum LayoutRulerOrientation
    * This is the enumeration of the ruler orientation types
    * @ingroup factory
    **/
    typedef enum
    {
      TypeRulerVertical = 0,
      TypeRulerHorizontal = 90
    } LayoutRulerOrientation;
  }
}
#endif //LAYOUT_ABSTRACT_ITEM_TYPE_H