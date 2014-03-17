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

#ifndef LAYOUT_ABSTRACT_ITEM_TYPE_H
#define LAYOUT_ABSTRACT_ITEM_TYPE_H

namespace te
{
  namespace layout
  {
    /** 
    * Enum TdkAbstractComponentType.
    * This is the enumeration of the components types
    * @ingroup factory
    **/
    typedef enum
    {
      TPRetangleItem,
      TPMapItem,
      TPPaperItem,
      TPLegendItem,
      TPPropertiesWindow,
      TPDisplayWindow,
      TPObjectInspectorWindow,
      TPToolbar,
      TPHorizontalRuler,
      TPVerticalRuler,
      TPText,
      TPItemGroup
    } LayoutAbstractType;

    /** 
    * Enum TdkAbstractComponentType.
    * This is the enumeration of the components types
    * @ingroup factory
    **/
    typedef enum
    {
      TPA0,
      TPA1,
      TPA2,
      TPA3,
      TPA4,
      TPA5
    } LayoutAbstractPaperType;

     /** 
    * Enum TdkAbstractComponentType.
    * This is the enumeration of the components types
    * @ingroup factory
    **/
    typedef enum
    {
      TPTopLeft,
      TPTopRight,
      TPLowerLeft,
      TPLowerRight,
      TPCenter,
      TPCenterRight,
      TPCenterLeft,
      TPNoneSide
    } LayoutSides;

    /** 
    * Enum TdkAbstractComponentType.
    * This is the enumeration of the components types
    * @ingroup factory
    **/
    typedef enum
    {
      TPMillimeter = 0,
      TPInch = 1
    } LayoutUnitsMetrics;
  }
}

#endif //LAYOUT_ABSTRACT_ITEM_TYPE_H