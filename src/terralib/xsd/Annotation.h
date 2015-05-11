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
  \file Annotation.h

  \brief A class that models a XSD annotation element.
*/

#ifndef __TERRALIB_XSD_INTERNAL_ANNOTATION_H
#define __TERRALIB_XSD_INTERNAL_ANNOTATION_H

// TerraLib
#include "Identifiable.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

// STL
#include <string>
#include <map>

namespace te
{
  namespace xsd
  {
// Forward declarations
    class AnnotationItem;

    /*!
      \class Annotation

      \brief A class that models a XSD annotation element.

      \sa AnnotationItem, AppInfo, Documentation, Identifiable, Annotated, Attribute

      \note Parent element: any element. 
     */
    class TEXSDEXPORT Annotation : public Identifiable
    {
      public:

        /*!
          \brief Constructor.
          
          \param id It specifies a unique ID for the annotation.

          \note The annotation will take the ownership of the id.
        */
        Annotation(std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        Annotation(const Annotation& rhs);

        /*! \brief Destructor. */
        ~Annotation();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        Annotation& operator=(const Annotation& rhs);
        
        /*!
          \brief It inserts the documentation (for human or for machine) into the annotation.

          \param item Some documentation.

          \pre Don't inform a NULL pointer

          \note The annotation will take the ownership of the given item.
        */
        void add(AnnotationItem* item);

        /*!
          \brief Add a property with non-schema namespace to the attribute.

          \param key The name of the property.
          \param value The value of the property.
        */
        //void addOtherAttribute(std::string key, std::string value);
        
        /*! \brief Returns a pointer to the vector of Annotation's items (AppInfos and/or Documentations) */
        const boost::ptr_vector<AnnotationItem>& getItems() const;

        /*!
          \brief It returns a map with other properties of the attribute element.

          \return Other properties of the attribute element as a key-value map.
        */
        //std::map<std::string, std::string>* getOtherAttributes() const;

      private:
        
        boost::ptr_vector<AnnotationItem> m_itemVec; //!< The list of comments for humans or for machines. (Optional)
        //std::map<std::string, std::string>* m_otherAttributes;  //!< A list of other attributes with non-schema namespace (Optional).
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_ANNOTATION_H

