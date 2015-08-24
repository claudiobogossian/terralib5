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
          \brief Returns value that represents printer mode type belonging to enumeration.
      
          \return enum value      
        */
        virtual EnumType* getModePrinter() const;
        /*!
          \brief Returns value that represents systematic scale mode type belonging to enumeration.
      
          \return enum value      
        */
        virtual EnumType* getModeSystematicScale() const;

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
        EnumType* m_modePrinter; //!< value that represents printer mode type belonging to enumeration
        EnumType* m_modeSystematicScale; //!< value that represents systematic scale mode type belonging to enumeration
        EnumType* m_modeUndo; //!< value that represents undo mode type belonging to enumeration
        EnumType* m_modeRedo; //!< value that represents redo mode type belonging to enumeration
        EnumType* m_modeTextEditorInteraction; //!< value that represents text editor type belonging to enumeration
        EnumType* m_modeArrowCursor; //!< value that represents arrow mode type belonging to enumeration
    };
  }
}

#endif

