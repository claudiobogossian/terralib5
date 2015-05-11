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
 \file terralib/metadata/iso19115/MD_DataIdentification.h
 
 \brief Information required to identify a dataset.
 */

#ifndef __TERRALIB_METADATA_INTERNAL_ISO19115_MD_DATAIDENTIFICATION_H
#define __TERRALIB_METADATA_INTERNAL_ISO19115_MD_DATAIDENTIFICATION_H

// TerraLib
#include "../../geometry/Envelope.h"
#include "../Config.h"
#include "CodeList.h"
#include "MD_Identification.h"

// STL
#include <vector>
#include <string>

namespace te
{
  namespace md
  {
    class CI_Citation;
    
    /*!
     \class MD_DataIdentification
     
     \brief Information required to identify a dataset.
     
     \note Only the mandatory elements for the core metadata are implemented.
     */
    class TEMDEXPORT MD_DataIdentification: public MD_Identification
    {
    public:
      
      /*! 
       \brief Constructor.
       \param citation Pointer to the citation to the resource. Class takes ownership. Do not pass null.
       \param abstract Brief narrative summary of the resource.
       \param status The development phase of the resource.
       \param lang A language used within the dataset. using standard ISO three-letter codes. Do not pass empty.
       */
      MD_DataIdentification(CI_Citation* cite, const std::string& abstract, MD_ProgressCode status, const std::string& lang);
      
      //! Destructor.
      ~MD_DataIdentification();
      
      /** @name Accessor methods
       *  Methods used to get or set properties.
       */
      //@{
      
      /*!
       \brief Adds a language used within the dataset.
       \param lang Language description using standard ISO three-letter codes. Do not pass empty string.
       */
      void addLanguage(const std::string& lang);
      
      //! Returns the set of languages associated to the dataset.
      const std::vector<std::string>& getLanguages() const;
      
      //! Adds a character coding standard in the dataset.
      void addCharacterSet(MD_CharacterSetCode code);
      
      //! Returns a character coding standard in the dataset.
      const std::vector<MD_CharacterSetCode>& getCharsetCodes() const;
      
      //! Adds a theme of the dataset.
      void addTopicCategory(MD_TopicCategoryCode code);
      
      //! Return the themes associated to the dataset.
      const std::vector<MD_TopicCategoryCode>& getTopicCategories() const;
            
      /*! 
       \brief Adds the spatial description of the dataset.
       \param llx Westernmost coordinate of the limit of the dataset extent, expressed in longitude in decimal degrees.
       \param lly Southernmost coordinate of the limit of the dataset extent, expressed in latitude in decimal degrees.
       \param urx Easternmost coordinate of the limit of the dataset extent, expressed in longitude in decimal degrees.
       \param lly Northernmost coordinate of the limit of the dataset extent, expressed in latitude in decimal degrees.
       \param isInside True if the bounding polygon encompasses an area covered by the data or false if an area where data is not present.  Default true.
       */
      void addGeographicBBoxExt(const double& llx, const double& lly,
                                const double& urx, const double& ury, 
                                bool isInside=true);
      
      /*! 
       \brief Adds the spatial description of the dataset.
       \param bbx Pointer to the enclosing boounding box. Do not pass null.
       \param isInside True if the bounding polygon encompasses an area covered by the data or false if an area where data is not present. Default true.
       */
       void addGeographicBBoxExt(const te::gm::Envelope* bb, bool isInside=true);
      
      //! Returns the set of extents for the dataset.
      const te::gm::Envelope& getExtent() const;
      
      //! Returns true if the bounding polygon encompasses an area covered by the data or false if is an area where data is not present.
      bool dataIsCovered() const;
      
      //! Sets the spatial representation type.
      void setSpatialRepType(te::md::MD_SpatialRepresentationTypeCode code);
      
      //! Returns the spatial representation type.
      te::md::MD_SpatialRepresentationTypeCode getSpatialRepTypeCode() const;
      
      /*! 
       \brief Sets resource scale.
       The scale of a map or other cartographic object expressed as a fraction or ratio which relates unit distance on the map or other cartographic object to distance, measured in the same units, on the ground.
       \param d The scale denominator. Do not pass <= 0;
       */
      void setScale(long d);
      
      //! Returns the scale information. If equal to 0 the denominator is not valid.
      long getScale() const;
      
      //@}
      
      MD_Identification* clone() const;
      
    private:
      
      std::vector<std::string> m_language;
      std::vector<MD_CharacterSetCode> m_charSet;
      std::vector<MD_TopicCategoryCode> m_topicCategory;
      te::gm::Envelope m_extent;
      bool m_dataIsCovered;
      te::md::MD_SpatialRepresentationTypeCode m_spatialRepresentationType;
      long m_scale; 
    };
  } // end namespace md
}   // end namespace te

#endif  // __TERRALIB_METADATA_INTERNAL_ISO19115_MD_DATAIDENTIFICATION_H

