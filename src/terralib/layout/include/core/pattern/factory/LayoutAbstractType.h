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
      TPLegendItem,
      TPPropertiesWindow,
      TPToolbar,
      TPHorizontalRuler,
      TPVerticalRuler,
      TPGroupRuler,
      TPText
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
  }
}

#endif //LAYOUT_ABSTRACT_ITEM_TYPE_H