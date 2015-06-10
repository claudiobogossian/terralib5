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
  \file PaperConfig.h
   
  \brief Class responsible for paper setting. Size, orientation, custom size, etc.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PAPER_CONFIG_H
#define __TERRALIB_LAYOUT_INTERNAL_PAPER_CONFIG_H

// TerraLib
#include "../core/enum/AbstractType.h"
#include "../../geometry/Envelope.h"
#include "Config.h"

namespace te
{
  namespace layout
  {
    /*!
      \brief Class responsible for paper setting. Size, orientation, custom size, etc.
	  
	    \ingroup layout
	  */
    class TELAYOUTEXPORT PaperConfig
    {
      public:

        /*!
          \brief Constructor
        */
        PaperConfig();

        /*!
          \brief Constructor

          \param paperType paper type. Ex.: A4
        */ 
        PaperConfig(LayoutAbstractPaperType paperType);

        /*!
          \brief Destructor
        */
        virtual ~PaperConfig();

        /*!
          \brief Custom paper size.
		  
		      \param w width
          \param h height
        */
        virtual void setPaperSizeCustom(double w, double h);

        /*!
          \brief Sets paper type enum. Ex.: A4.
		  
		      \param paperType paper type enum
        */
        virtual void setPaperType(LayoutAbstractPaperType paperType);

        /*!
          \brief Returns paper type.
		  
		      \param paper type enum 	  
        */
        virtual LayoutAbstractPaperType getPaperType();

        /*!
          \brief Returns paper size. Height and Width.
		  
		      \param w returns width
          \param h returns height
        */
        virtual void getPaperSize(double &w, double &h);

        /*!
          \brief Sets paper orientation type enum. Ex.: Portrait.
		  
		      \param paper orientation type enum
        */
        virtual void setPaperOrientation(LayoutOrientationType orientation);

        /*!
          \brief Returns paper orientation type enum.
		  
		      \param paper orientation type enum
        */
        virtual LayoutOrientationType getPaperOrientantion();
        
      protected:

        LayoutAbstractPaperType m_paperType; //!< paper type enum. Ex.: A4
        LayoutOrientationType   m_paperOrientationType; //!< paper orientation type enum. Ex.: Portrait
        double                  m_customW; //!< custom paper size width
        double                  m_customH; //!< custom paper size height
    };
  }
}

#endif
