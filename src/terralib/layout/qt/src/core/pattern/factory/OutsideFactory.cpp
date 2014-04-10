/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file OutsideFactory.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "OutsideFactory.h"
#include "AbstractType.h"
#include "OutsideParamsCreate.h"
#include "ToolbarOutside.h"
#include "PropertiesOutside.h"
#include "DisplayOutside.h"
#include "ObjectInspectorOutside.h"

te::layout::OutsideFactory::OutsideFactory()
{

}

te::layout::OutsideFactory::~OutsideFactory()
{

}

te::layout::Observer* te::layout::OutsideFactory::make( LayoutAbstractObjectType type, OutsideParamsCreate params /*= LayoutItemParamsCreate()*/ )
{
	Observer* outside = 0;

	switch (type)
	{

	case TPToolbar:
		{
			ToolbarOutside* toolbar = new ToolbarOutside(params.getController(), params.getModel());		
			outside = (Observer*)toolbar;
			return outside;
			break;
		}
	case TPPropertiesWindow:
		{
			PropertiesOutside* window = new PropertiesOutside(params.getController(), params.getModel());		
			outside = (Observer*)window;
			return outside;
			break;
		}
  case TPObjectInspectorWindow :
    {
      ObjectInspectorOutside* inspector = new ObjectInspectorOutside(params.getController(), params.getModel());		
      outside = (Observer*)inspector;
      return outside;
      break;
    }
  case TPDisplayWindow:
    {
      DisplayOutside* displayWindow = new DisplayOutside(params.getController(), params.getModel());		
      outside = (Observer*)displayWindow;
      return outside;
      break;
    }
	}

	return 0;
}
