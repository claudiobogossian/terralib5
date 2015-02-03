/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file ImageModel.h
   
   \brief Class that represents a "Model" part of Image MVC component. 
      Its coordinate system is the same of scene (millimeters). 
      This is also son of ItemModelObservable, so it can become observable.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_IMAGE_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_IMAGE_MODEL_H

// TerraLib
#include "../core/pattern/mvc/ItemModelObservable.h"
#include "../core/ContextItem.h"
#include "../../maptools/Enums.h"
#include "../core/Config.h"

// STL
#include <string>

namespace te
{
  namespace layout
  {
    /*!
      \brief Class that represents a "Model" part of Image MVC component. 
          Its coordinate system is the same of scene (millimeters). 
          This is also son of ItemModelObservable, so it can become observable.
      	  
	    \ingroup layout

      \sa te::layout::ItemModelObservable
	  */
    class TELAYOUTEXPORT ImageModel : public ItemModelObservable
    {
      public:

        /*!
          \brief Constructor
        */
        ImageModel();

        /*!
          \brief Destructor
        */ 
        virtual ~ImageModel();

        /*!
          \brief Reimplemented from ItemModelObservable
         */
        virtual te::layout::Properties* getProperties() const;

        /*!
          \brief Reimplemented from ItemModelObservable
         */
        virtual void updateProperties(te::layout::Properties* properties);   

        /*!
          \brief Reimplemented from ItemModelObservable
         */
        virtual void draw( ContextItem context );

        /*!
          \brief Change file name

          \param fileName file name
         */
        virtual void setFileName(std::string fileName);

        /*!
          \brief Change file name

          \param fileName file name
         */
        virtual std::string getFileName();
        
        /*!
          \brief Returns the file extension.

          \param file extension
         */
        virtual std::string getFileExtension();

        /*!
          \brief Returns the type of the image.

          \return type of the image
         */
        virtual te::map::ImageType getFileType();

      protected:
        
        std::string m_fileName; //!< full path where the file is
        std::string m_fileExtension; //!< file extension
        te::map::ImageType m_imgType; //!< image type
    };
  }
}

#endif 
