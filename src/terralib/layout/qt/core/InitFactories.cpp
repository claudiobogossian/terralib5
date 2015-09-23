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
\file terralib/layout/qt/core/InitFactories.cpp

\brief Initialize all tool factories.
*/

// TerraLib
#include "InitFactories.h"
#include "pattern/factory/tool/ZoomAreaToolFactory.h"
#include "pattern/factory/item/RectangleItemFactory.h"
#include "pattern/factory/tool/CreateLineItemToolFactory.h"
#include "pattern/factory/tool/CreatePolygonItemToolFactory.h"
#include "pattern/factory/tool/CreateItemToolFactory.h"
#include "pattern/factory/item/ArrowItemFactory.h"
#include "pattern/factory/item/BalloonItemFactory.h"
#include "pattern/factory/item/BarCodeItemFactory.h"
#include "pattern/factory/item/EllipseItemFactory.h"
#include "pattern/factory/item/GridGeodesicItemFactory.h"
#include "pattern/factory/item/GridMapItemFactory.h"
#include "pattern/factory/item/GridPlanarItemFactory.h"
#include "pattern/factory/item/ImageItemFactory.h"
#include "pattern/factory/item/ItemGroupItemFactory.h"
#include "pattern/factory/item/LegendChildItemFactory.h"
#include "pattern/factory/item/LegendItemFactory.h"
#include "pattern/factory/item/MapItemFactory.h"
#include "pattern/factory/item/MapLocationItemFactory.h"
#include "pattern/factory/item/MovingItemGroupItemFactory.h"
#include "pattern/factory/item/NorthItemFactory.h"
#include "pattern/factory/item/PointItemFactory.h"
#include "pattern/factory/item/ScaleItemFactory.h"
#include "pattern/factory/item/SVGItemFactory.h"
#include "pattern/factory/item/TextGridItemFactory.h"
#include "pattern/factory/item/TextItemFactory.h"
#include "pattern/factory/item/TitleItemFactory.h"
#include "pattern/factory/item/PolygonItemFactory.h"
#include "pattern/factory/item/LineItemFactory.h"
#include "pattern/factory/item/PaperItemFactory.h"
#include "pattern/factory/outside/ColorDialogOutsideFactory.h"
#include "pattern/factory/outside/EditTemplateOutsideFactory.h"
#include "pattern/factory/outside/FontDialogOutsideFactory.h"
#include "pattern/factory/outside/GridSettingsOutsideFactory.h"
#include "pattern/factory/outside/LegendChoiceOutsideFactory.h"
#include "pattern/factory/outside/MapLayerChoiceOutsideFactory.h"
#include "pattern/factory/outside/ObjectInspectorOutsideFactory.h"
#include "pattern/factory/outside/PageSetupOutsideFactory.h"
#include "pattern/factory/outside/PropertiesOutsideFactory.h"
#include "pattern/factory/outside/SVGDialogOutsideFactory.h"
#include "pattern/factory/outside/SystematicScaleOutsideFactory.h"
#include "pattern/factory/outside/TextGridSettingsOutsideFactory.h"
#include "pattern/factory/outside/ToolbarOutsideFactory.h"

namespace te
{
  namespace layout
  {
    void TELAYOUTEXPORT initToolFactories()
    {
      static bool ToolFactoryInitalized = false;

      if (!ToolFactoryInitalized)
      {
        ToolFactoryInitalized = true;
        static ZoomAreaToolFactory ZoomAreaToolFactory_instance;
        static CreateLineItemToolFactory CreateLineItemToolFactory_instance;
        static CreatePolygonItemToolFactory CreatePolygonItemToolFactory_instance;
        static CreateItemToolFactory CreateItemToolFactory_instance;
      }

      // Temporary
      static bool OutsideFactoryInitalized = false;
      if (!OutsideFactoryInitalized)
      {
        OutsideFactoryInitalized = true;
        static ColorDialogOutsideFactory ColorDialogOutsideFactory_instance;
        static EditTemplateOutsideFactory EditTemplateOutsideFactory_instance;
        static FontDialogOutsideFactory FontDialogOutsideFactory_instance;
        static GridSettingsOutsideFactory GridSettingsOutsideFactory_instance;
        static LegendChoiceOutsideFactory LegendChoiceOutsideFactory_instance;
        static MapLayerChoiceOutsideFactory MapLayerChoiceOutsideFactory_instance;
        static ObjectInspectorOutsideFactory ObjectInspectorOutsideFactory_instance;
        static PageSetupOutsideFactory PageSetupOutsideFactory_instance;
        static PropertiesOutsideFactory PropertiesOutsideFactory_instance;
        static SVGDialogOutsideFactory SVGDialogOutsideFactory_instance;
        static SystematicScaleOutsideFactory SystematicScaleOutsideFactory_instance;
        static TextGridSettingsOutsideFactory TextGridSettingsOutsideFactory_instance;
        static ToolbarOutsideFactory ToolbarOutsideFactory_instance;
      }
    }

    void TELAYOUTEXPORT initItemFactories()
    {
      static bool ItemFactoryInitalized = false;

      if (!ItemFactoryInitalized)
      {
        ItemFactoryInitalized = true;
        static RectangleItemFactory RectangleItemFactory_instance;
        static ArrowItemFactory ArrowItemFactory_instance;
        static BalloonItemFactory BalloonItemFactory_instance;
        static BarCodeItemFactory BarCodeItemFactory_instance;
        static EllipseItemFactory EllipseItemFactory_instance;
        static GridGeodesicItemFactory GridGeodesicItemFactory_instance;
        static GridMapItemFactory GridMapItemFactory_instance;
        static GridPlanarItemFactory GridPlanarItemFactory_instance;
        static ImageItemFactory ImageItemFactory_instance;
        static ItemGroupItemFactory ItemGroupItemFactory_instance;
        static LegendChildItemFactory LegendChildItemFactory_instance;
        static LegendItemFactory LegendItemFactory_instance;
        static MapItemFactory MapItemFactory_instance;
        static MapLocationItemFactory MapLocationItemFactory_instance;
        static MovingItemGroupItemFactory MovingItemGroupItemFactory_instance;
        static NorthItemFactory NorthItemFactory_instance;
        static PointItemFactory PointItemFactory_instance;
        static ScaleItemFactory ScaleItemFactory_instance;
        static SVGItemFactory SVGItemFactory_instance;
        static TextGridItemFactory TextGridItemFactory_instance;
        static TextItemFactory TextItemFactory_instance;
        static TitleItemFactory TitleItemFactory_instance;
        static PolygonItemFactory PolygonItemFactory_instance;
        static LineItemFactory LineItemFactory_instance;
        static PaperItemFactory PaperItemFactory_instance;
      }
    }
  }
}


