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
  \file terralib/ws/ogc/wcs/client/XMLParser.h

  \brief Parse the WCS XML to a WCS struct

  \author Vinicius Campanha
*/

#ifndef __TERRALIB_WS_OGC_WCS_XMLPARSER_WCS_H
#define __TERRALIB_WS_OGC_WCS_XMLPARSER_WCS_H

// STL
#include <memory>
#include <vector>

// TerraLib
#include "../../../../xml.h"


namespace te
{
  namespace ws
  {
    namespace ogc
    {
      struct Capabilities
      {
        std::vector< std::string > operations;
        std::vector< std::string > coverages;
      };

      struct DomainSet
      {
        std::vector< std::string > axisLabels;
        std::vector< std::string > low;
        std::vector< std::string > high;
      };

      struct BoundedBy
      {
        std::string srsName;
        std::string axisLabels;
        std::string uomLabels;
        std::string srsDimension;
        std::vector< std::string > lowerCorner;
        std::vector< std::string > upperCorner;
      };

      struct ServiceParameters
      {
        std::string coverageSubtype;
        std::string coverageSubtypeParent;
        std::string nativeFormat;
        std::string extension;
      };

      struct CoverageDescription
      {
        std::string coverageId;
        struct BoundedBy boundedBy;
        struct DomainSet domainSet;
        struct ServiceParameters serviceParameters;
        std::vector< std::string > extension;
      };

      static Capabilities parseCapabilities(const std::string xmlPath)
      {
        // Read XML file
        std::auto_ptr<te::xml::Reader> reader(te::xml::ReaderFactory::make("XERCES"));
        reader->setValidationScheme(false);
        reader->setIgnoreWhiteSpaces(true);

        reader->read(xmlPath);

        // Validate the XML file
        if(!reader->next())
        {
          throw te::common::Exception(TE_TR("Can not read the Capabilities XML file!"));
        }

        if(reader->getElementLocalName().compare("Capabilities") != 0)
        {
          throw te::common::Exception(TE_TR("Can not find Capabilities on the received XML!"));
        }

        Capabilities capabilities;
        while(reader->next())
        {
          if(reader->getNodeType() == te::xml::START_ELEMENT && reader->hasAttrs() && reader->getElementLocalName().compare("Operation") == 0)
          {
            capabilities.operations.push_back(reader->getAttr(0));
          }
          else if (reader->getNodeType() == te::xml::VALUE && reader->getElementLocalName().compare("CoverageId") == 0)
          {
            capabilities.coverages.push_back(reader->getElementValue());
          }
        }

        return capabilities;
      }

      static CoverageDescription parseDescribeCoverage(const std::string xmlPath)
      {
        // Read XML file
        std::auto_ptr<te::xml::Reader> reader(te::xml::ReaderFactory::make("XERCES"));
        reader->setValidationScheme(false);
        reader->setIgnoreWhiteSpaces(true);

        reader->read(xmlPath);

        // Validate the XML file
        if(!reader->next())
        {
          throw te::common::Exception(TE_TR("Can not read the Coverage Descriptions XML file!"));
        }

        if(reader->getElementLocalName().compare("CoverageDescriptions") != 0)
        {
          throw te::common::Exception(TE_TR("Can not find Coverage Descriptions on the received XML!"));
        }

        te::ws::ogc::CoverageDescription coverageDescription;
        te::ws::ogc::BoundedBy boundedBy;
        te::ws::ogc::DomainSet domainSet;
        te::ws::ogc::ServiceParameters serviceParameters;

        while(reader->next())
        {
          std::string elementLocalName = reader->getElementLocalName();

          if(reader->getNodeType() == te::xml::START_ELEMENT && reader->hasAttrs())
          {
            if(elementLocalName.compare("Envelope") == 0)
            {
              for(unsigned int i = 0; i < reader->getNumberOfAttrs(); i++)
              {
                std::string name = reader->getAttrLocalName(i);

                if(name.compare("srsName") == 0) boundedBy.srsName = reader->getAttr(i);
                if(name.compare("axisLabels") == 0) boundedBy.axisLabels = reader->getAttr(i);
                if(name.compare("uomLabels") == 0) boundedBy.uomLabels = reader->getAttr(i);
                if(name.compare("srsDimension") == 0) boundedBy.srsDimension = reader->getAttr(i);
              }
            }
          }
          else if(reader->getNodeType() == te::xml::VALUE)
          {
            // SWITCH CASE
            if(elementLocalName.compare("lowerCorner") == 0) boundedBy.lowerCorner.push_back(reader->getElementValue());
            if(elementLocalName.compare("upperCorner") == 0) boundedBy.upperCorner.push_back(reader->getElementValue());

            if(elementLocalName.compare("axisLabels") == 0) domainSet.axisLabels.push_back(reader->getElementValue());
            if(elementLocalName.compare("low") == 0) domainSet.low.push_back(reader->getElementValue());
            if(elementLocalName.compare("high") == 0) domainSet.high.push_back(reader->getElementValue());

            if(elementLocalName.compare("CoverageSubtype") == 0) serviceParameters.coverageSubtype = reader->getElementValue();
            if(elementLocalName.compare("coverageSubtypeParent") == 0) serviceParameters.coverageSubtypeParent = reader->getElementValue();
            if(elementLocalName.compare("nativeFormat") == 0) serviceParameters.nativeFormat = reader->getElementValue();
            if(elementLocalName.compare("extension") == 0) serviceParameters.extension = reader->getElementValue();
         }
        }

        coverageDescription.boundedBy = boundedBy;
        coverageDescription.domainSet = domainSet;
        coverageDescription.serviceParameters = serviceParameters;

        return coverageDescription;
      }

    }
  }
}

#endif // __TERRALIB_WS_OGC_WCS_XMLPARSER_WCS_H
