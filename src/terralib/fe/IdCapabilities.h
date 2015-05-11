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
  \file IdCapabilities.h
  
  \brief Id capabilities include the ability to refer to elements in a GML version 3 data source using an ogc:GmlObjectId with a gml:id attribute.  
 */

#ifndef __TERRALIB_FE_INTERNAL_IDCAPABILITIES_H
#define __TERRALIB_FE_INTERNAL_IDCAPABILITIES_H

// TerraLib
#include "Config.h"

namespace te
{
  namespace fe
  {
    /*!
      \class IdCapabilities
      
      \brief Id capabilities include the ability to refer to elements in a GML version 3 data source using an ogc:GmlObjectId with a gml:id attribute.

      Any elements with a gml:id attribute, including
      features, geometries, topologies, and complex attributes,
      may be so referenced.<br>
      For backward compatibility, Id capabilities
      also include the ability to refer to features
      using an ogc:FeatureId with a fid attribute.
      Only features with a GML version 2 gml:fid attribute
      may be so referenced. Note that features in a
      GML version 3 data source may have gml:fid
      attributes for backwards compatibility with GML
      version 2, in addition to gml:id attributes
      introduced in GML version 3. This ability is
      deprecated.

      \sa FilterCapabilities
     */
    class TEFEEXPORT IdCapabilities
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new IdCapabilities. */
        IdCapabilities();

        /*! \brief Destructor. */
        ~IdCapabilities();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns true if GML 3 identification is supported, otherwise, it returns false.

          \return True if GML 3 identification is supported otherwise it returns false.
         */              
        const bool& hasEID() const;

        /*! \brief It enables GML 3 identification support. */
        void enableEID();

        /*! \brief It disables GML 3 identification support. */
        void disableEID();

        /*!
          \brief It returns true if GML 2 identification is supported, otherwise, it returns false.

          \return True if GML 2 identification is supported otherwise it returns false.
         */
        const bool& hasFID() const;

        /*! \brief It enables GML 2 identification support. */
        void enableFID();

        /*! \brief It disables GML 2 identification support. */
        void disableFID();

        //@}

      private:

        bool m_hasEID; //!< GML version 3 data source may have gml:id.
        bool m_hasFID; //!< GML version 2 gml:fid attribute.
    };    

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_IDCAPABILITIES_H
