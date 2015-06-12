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
  \file EnumDataType.h
   
  \brief Class to represent a data type enumeration. Ex.: int, double, bool, te::color::RGBAColor (color), etc.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ENUM_DATATYPE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ENUM_DATATYPE_H

// TerraLib
#include "AbstractEnum.h"
#include "../Config.h"

namespace te
{
  namespace layout
  {
    class EnumType;

    /*!
      \brief Class to represent a data type enumeration. Ex.: int, double, bool, te::color::RGBAColor (color), etc.
	  
	    \ingroup layout

	    \sa te::layout::AbstractEnum
	  */
    class TELAYOUTEXPORT EnumDataType : public AbstractEnum
    {
      public:

        /*!
          \brief Constructor
        */
        EnumDataType(); 

        /*!
          \brief Destructor
        */ 
        virtual ~EnumDataType();
        
        /*!
          \brief Returns value that represents type none belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getDataTypeNone() const;

        /*!
          \brief Returns value that represents type integer belonging to enumeration.
		  
		      \return enum value   	  
        */
        virtual EnumType* getDataTypeInt() const;

        /*!
          \brief Returns value that represents type double belonging to enumeration. 
		  
		      \return enum value  	  
        */
        virtual EnumType* getDataTypeDouble() const;

        /*!
          \brief Returns value that represents type long belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getDataTypeLong() const;

        /*!
          \brief Returns value that represents type float belonging to enumeration.
		  
		      \return enum value   	  
        */
        virtual EnumType* getDataTypeFloat() const;

        /*!
          \brief Returns value that represents type string belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getDataTypeString() const;

        /*!
          \brief Returns value that represents type bool belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getDataTypeBool() const;

        /*!
          \brief Returns value that represents type te::color::RGBAColor** (color) belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getDataTypeColor() const;

        /*!
          \brief Returns value that represents type GridSettings (string) belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getDataTypeGridSettings() const;

        /*!
          \brief Returns value that represents type StringList (string) belonging to enumeration.
		  
		      \return enum value   	  
        */
        virtual EnumType* getDataTypeStringList() const;

        /*!
          \brief Returns value that represents type Font belonging to enumeration.
		  
		      \return enum value   	  
        */
        virtual EnumType* getDataTypeFont() const;

        /*!
          \brief Returns value that represents type image (string) belonging to enumeration.
		  
		      \return enum value   	  
        */
        virtual EnumType* getDataTypeImage() const;

        /*!
          \brief Returns value that represents type TextGridSettings (string) belonging to enumeration.
		  
		      \return enum value   	  
        */
        virtual EnumType* getDataTypeTextGridSettings() const;

        /*!
          \brief Returns value that represents type Group (string) belonging to enumeration.
		  
		      \return enum value   	  
        */
        virtual EnumType* getDataTypeGroup() const;

        /*!
          \brief Returns value that represents type Map Choice (string) belonging to enumeration.
		  
		      \return enum value   	  
        */
        virtual EnumType* getDataTypeMapChoice() const;

        /*!
          \brief Returns value that represents type Legend Choice (string) belonging to enumeration.
		  
		      \return enum value   	  
        */
        virtual EnumType* getDataTypeLegendChoice() const;

        /*!
          \brief Returns value that represents type Generic Vector (T) belonging to enumeration.
		  
		      \return enum value   	  
        */
        virtual EnumType* getDataTypeGenericVariant() const;

        /*!
          \brief Returns value that represents type Layer (te::map::AbstractLayerPtr) belonging to enumeration.
		  
		      \return enum value   	  
        */
        virtual EnumType* getDataTypeLayerList() const;

        /*!
          \brief Returns value that represents type string vector (std::vector<std::string>) belonging to enumeration
		  
		      \return enum value   	  
        */
        virtual EnumType* getDataTypeStringVector() const;

        /*!
          \brief Returns value that represents type SVGView(string) belonging to enumeration.
		  
		      \return enum value   	  
        */
        virtual EnumType* getDataTypeSVGView() const;

      protected:

        /*!
          \brief Reimplemented from AbstractEnum
        */
        virtual void init();

      protected:

        EnumType* m_dataTypeNone; //!< value that represents type none belonging to enumeration
        EnumType* m_dataTypeInt; //!< value that represents type integer belonging to enumeration
        EnumType* m_dataTypeDouble; //!< value that represents type double belonging to enumeration
        EnumType* m_dataTypeLong; //!< value that represents type long belonging to enumeration
        EnumType* m_dataTypeFloat; //!< value that represents type float belonging to enumeration
        EnumType* m_dataTypeString; //!< value that represents type string belonging to enumeration
        EnumType* m_dataTypeBool; //!< value that represents type bool belonging to enumeration
        EnumType* m_dataTypeColor; //!< value that represents type te::color::RGBAColor** (color) belonging to enumeration
        EnumType* m_dataTypeGridSettings; //!< value that represents type GridSettings (string) belonging to enumeration
        EnumType* m_dataTypeStringList; //!< value that represents type StringList (string) belonging to enumeration.
        EnumType* m_dataTypeFont; //!< value that represents type Font belonging to enumeration
        EnumType* m_dataTypeImage; //!< value that represents type image (string) belonging to enumeration
        EnumType* m_dataTypeTextGridSettings; //!< value that represents type TextGridSettings (string) belonging to enumeration
        EnumType* m_dataTypeGroup; //!< value that represents type Group (string) belonging to enumeration
        EnumType* m_dataTypeMapChoice; //!< value that represents type Map Choice (string) belonging to enumeration
        EnumType* m_dataTypeLegendChoice; //!< value that represents type Legend Choice (string) belonging to enumeration
        EnumType* m_dataTypeGenericVariant; //!< value that represents type Generic Vector belonging to enumeration
        EnumType* m_dataTypeLayerList; //!< value that represents type Layer List (te::map::AbstractLayerPtr) belonging to enumeration
        EnumType* m_dataTypeStringVector; //!< value that represents type string vector (std::vector<std::string>) belonging to enumeration
        EnumType* m_dataTypeSVGView; //!< value that represents type SVGView (string) belonging to enumeration
    };
  }
}

#endif



