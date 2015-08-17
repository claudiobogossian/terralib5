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
	\file terralib/layout/qt/pattern/factory/ToolFactory.h

	\brief This is the abstract factory for tools.
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TOOL_FACTORY_H 
#define __TERRALIB_LAYOUT_INTERNAL_TOOL_FACTORY_H

// TerraLib
#include "../../../../../../common/ParameterizedAbstractFactory.h"
#include "../../../../../core/Config.h"
#include "../../../tools/AbstractLayoutTool.h"
#include "ToolFactoryParamsCreate.h"

namespace te
{
  namespace layout
  {
		/*!
			\class ToolFactory

			\brief This is the abstract factory for tools.

			It will create objects of type AbstractLayoutTool and will pass
			parameters of type ToolFactoryParamsCreate to their factories constructors.

			If you want a new tool you can use a command like:
			<code>
			  te::layout::AbstractLayoutTool* pEngine = te::layout::ToolFactory::make("PANTOOL");
			</code>
			Or
			<code>
				te::layout::EnumToolType* tool = Enums::getInstance().getEnumToolType();
				te::layout::EnumType* panTool = tool->getToolTypePan();
				te::layout::AbstractLayoutTool* pEngine = te::layout::ToolFactory::make(panTool->getName());
			</code>

			\note The caller of the method make will take the ownership of the returned tool.

			\sa AbstractLayoutTool, AbstractFactory
		*/
		class TELAYOUTEXPORT ToolFactory : public te::common::ParameterizedAbstractFactory<AbstractLayoutTool, std::string, ToolFactoryParamsCreate>
		{
			public:

				/*! \brief Virtual destructor. */
				virtual ~ToolFactory();

			protected:

				/*!
				\brief It creates the factory.

				The key of a ToolFactory is a string.

				\param factoryKey The key that identifies the factory.
				*/
				ToolFactory(const std::string& factoryKey);
    };
  }
}

#endif
