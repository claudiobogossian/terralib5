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

#include "XMLParser.h"

#include <iostream>

//BOOST
#include <boost/algorithm/string.hpp>

//TerraLib
#include "../../../../core/uri/URI.h"
#include "../../../../common/StringUtils.h"


te::ws::ogc::wcs::XMLParser::XMLParser()
{

}

te::ws::ogc::wcs::XMLParser::~XMLParser()
{

}

te::ws::ogc::wcs::Capabilities te::ws::ogc::wcs::XMLParser::parseCapabilities(const std::string& xmlPath)
{

  // Read XML file
  std::auto_ptr<te::xml::Reader> reader(te::xml::ReaderFactory::make("XERCES"));
  reader->setValidationScheme(false);
  reader->setDoSchema(false);
  reader->setIgnoreWhiteSpaces(true);

  reader->read(xmlPath);

  // Validate the XML file
  if(!reader->next())
  {
    throw te::common::Exception(TE_TR("Can not read the Capabilities XML file!"));
  }

  if(!boost::iequals(reader->getElementLocalName(), "Capabilities"))
  {
    throw te::common::Exception(TE_TR("Can not find Capabilities on the received XML!"));
  }

  Capabilities capabilities;
  while(reader->next())
  {
    if(reader->getNodeType() == te::xml::START_ELEMENT)
    {
      if(boost::iequals(reader->getElementLocalName(), "OperationsMetadata"))
      {
        while(reader->next() && !(te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "OperationsMetadata")))
        {
          if(reader->getNodeType() == te::xml::START_ELEMENT && boost::iequals(reader->getElementLocalName(), "Operation") && reader->hasAttrs())
          {
            capabilities.operations.push_back(reader->getAttr(0));
          }
        }
      }
      else if(boost::iequals(reader->getElementLocalName(), "Contents"))
      {
        while(reader->next() && !(te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "Contents")))
        {
          if(boost::iequals(reader->getElementLocalName(), "CoverageSummary"))
          {
            while(reader->next() && !(te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "CoverageSummary")))
            {
              if(reader->getNodeType() == te::xml::VALUE && boost::iequals(reader->getElementLocalName(), "CoverageId"))
              {
                capabilities.coverages.push_back(reader->getElementValue());
              }
            }
          }
        }
      }
    }
  }
  return capabilities;
}

te::ws::ogc::wcs::CoverageDescription te::ws::ogc::wcs::XMLParser::parseDescribeCoverage(const std::string& xmlPath)
{
  // Read XML file
  std::auto_ptr<te::xml::Reader> reader(te::xml::ReaderFactory::make("XERCES"));
  reader->setValidationScheme(false);
  reader->setDoSchema(false);
  reader->setIgnoreWhiteSpaces(true);

  reader->read(xmlPath);

  // Validate the XML file
  if(!reader->next())
  {
    throw te::common::Exception(TE_TR("Can not read the Coverage Descriptions XML file!"));
  }

  if(!boost::algorithm::iequals(reader->getElementLocalName(), "CoverageDescriptions"))
  {
    throw te::common::Exception(TE_TR("Can not find Coverage Descriptions on the received XML!"));
  }

  te::ws::ogc::wcs::CoverageDescription coverageDescription;
  te::ws::ogc::wcs::EnvelopeWithTimePeriod envelope;
  te::ws::ogc::wcs::DomainSet domainSet;
  te::ws::ogc::wcs::ServiceParameters serviceParameters;
  std::vector< std::string > fieldNames;
  te::ws::ogc::wcs::TimeDomain timeDomain;

  while(reader->next())
  {
    if(reader->getNodeType() == te::xml::START_ELEMENT)
    {
      if(boost::iequals(reader->getElementLocalName(), "CoverageId"))
      {
        while(reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "CoverageId")))
        {
          if(reader->getNodeType() == te::xml::VALUE)
          {
            if(boost::iequals(reader->getElementLocalName(), "CoverageId"))
              coverageDescription.coverageId = reader->getElementValue();
          }
        }
      }
      else if(boost::algorithm::iequals(reader->getElementLocalName(), "boundedBy"))
      {
        while(reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "boundedBy")))
        {
          if((boost::iequals(reader->getElementLocalName(), "Envelope") || boost::iequals(reader->getElementLocalName(), "EnvelopeWithTimePeriod"))  && reader->hasAttrs())
          {
            std::stringstream axis, uom;
            for(unsigned int i = 0; i < reader->getNumberOfAttrs(); i++)
            {
              if(boost::iequals(reader->getAttrLocalName(i), "srsName"))
              {
                envelope.srsName = reader->getAttr(i);

                te::core::URI url (envelope.srsName);

                if (url.isValid())
                {
                  std::string path = url.path();
                  std::vector<std::string> elements = te::common::SplitString(path, '/');

                  std::string code = elements.back(); // getting SRS code.
                  elements.pop_back(); // removing code.
                  elements.pop_back(); // removing version.
                  std::string auth = elements.back(); // getting authority eg.: EPSG

                  envelope.srsName = auth + ":" + code;
                }

              }

              else if(boost::iequals(reader->getAttrLocalName(i), "axisLabels"))
                axis.str(reader->getAttr(i));
              else if(boost::iequals(reader->getAttrLocalName(i), "uomLabels"))
                uom.str(reader->getAttr(i));
              else if(boost::iequals(reader->getAttrLocalName(i), "srsDimension"))
                envelope.srsDimension = reader->getAttr(i);
            }

            if(axis.good())
            {
              std::string part;
              axis >> part;
              envelope.firstLabel = part;

              if(axis.good())
              {
                axis >> part;
                envelope.secondLabel = part;
              }

              if(axis.good())
              {
                axis >> part;
                envelope.timeLabel = part;
              }
            }

            while(uom.good())
            {
              std::string part;
              uom >> part;
              envelope.uomLabels.push_back(part);
            }

            std::stringstream lower, upper;

            while(reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT
                                      && (boost::iequals(reader->getElementLocalName(), "Envelope")
                                          || boost::iequals(reader->getElementLocalName(), "EnvelopeWithTimePeriod"))))
            {

              if(reader->getNodeType() == te::xml::VALUE)
              {
                if(boost::iequals(reader->getElementLocalName(), "lowerCorner"))
                  lower.str(reader->getElementValue());
                else if(boost::iequals(reader->getElementLocalName(), "upperCorner"))
                  upper.str(reader->getElementValue());
                else if(boost::iequals(reader->getElementLocalName(), "beginPosition"))
                  envelope.beginPosition = reader->getElementValue();
                else if(boost::iequals(reader->getElementLocalName(), "endPosition"))
                  envelope.endPosition = reader->getElementValue();
              }
            }

            if(lower.good())
            {
              std::string part;
              lower >> part;
              envelope.lowerCorner_X = part;

              if(lower.good())
              {
                lower >> part;
                envelope.lowerCorner_Y = part;
              }
            }

            if(upper.good())
            {
              std::string part;
              upper >> part;
              envelope.upperCorner_X = part;

              if(upper.good())
              {
                upper >> part;
                envelope.upperCorner_Y = part;
              }
            }
          }
        }
      }
      else if(boost::iequals(reader->getElementLocalName(), "metadata"))
      {
        reader->next();

        if(boost::iequals(reader->getElementLocalName(), "Extension"))
        {
          reader->next();

          if(boost::iequals(reader->getElementLocalName(), "TimeDomain"))
          {
            timeDomain = this->parseTimeDomain(reader);
          }
        }
      }
      else if(boost::iequals(reader->getElementLocalName(), "domainSet"))
      {
        std::stringstream axis;
        std::stringstream low;
        std::stringstream high;

        while(reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "domainSet")))
        {
          if(reader->getNodeType() == te::xml::VALUE)
          {
            if(boost::iequals(reader->getElementLocalName(), "axisLabels"))
              axis.str(reader->getElementValue());
            else if(boost::iequals(reader->getElementLocalName(), "low"))
              low.str(reader->getElementValue());
            else if(boost::iequals(reader->getElementLocalName(), "high"))
              high.str(reader->getElementValue());
          }
        }

        SubSet subSet;

        while(axis.good())
        {
          std::string element;
          axis >> element;
          subSet.axis = element;

          if(low.good())
          {
            std::string element;
            low >> element;
            subSet.min = element;
          }

          if(high.good())
          {
            std::string element;
            high >> element;
            subSet.max = element;
          }

          domainSet.subSet.push_back(subSet);
        }
      }
      else if(boost::iequals(reader->getElementLocalName(), "rangeType"))
      {
        while(reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "rangeType")))
        {
          if(boost::iequals(reader->getElementLocalName(), "field") && reader->hasAttrs())
          {
            for(unsigned int i = 0; i < reader->getNumberOfAttrs(); i++)
            {
              if(boost::iequals(reader->getAttrLocalName(i), "name"))
                fieldNames.push_back(reader->getAttr(i));
            }
          }
        }
      }
      else if(boost::iequals(reader->getElementLocalName(), "ServiceParameters"))
      {
        while(reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "ServiceParameters")))
        {
          if(reader->getNodeType() == te::xml::VALUE)
          {
            if(boost::iequals(reader->getElementLocalName(), "CoverageSubtype"))
              serviceParameters.coverageSubtype = reader->getElementValue();
            else if(boost::iequals(reader->getElementLocalName(), "coverageSubtypeParent"))
              serviceParameters.coverageSubtypeParent = reader->getElementValue();
            else if(boost::iequals(reader->getElementLocalName(), "nativeFormat"))
              serviceParameters.nativeFormat = reader->getElementValue();
            else if(boost::iequals(reader->getElementLocalName(), "extension"))
              serviceParameters.extension = reader->getElementValue();
          }
        }
      }
    }
  }

  coverageDescription.envelope = envelope;
  coverageDescription.domainSet = domainSet;
  coverageDescription.serviceParameters = serviceParameters;
  coverageDescription.fieldNames = fieldNames;
  coverageDescription.timeDomain = timeDomain;

  return coverageDescription;
}

te::ws::ogc::wcs::TimeDomain te::ws::ogc::wcs::XMLParser::parseTimeDomain(const std::auto_ptr<te::xml::Reader> &reader)
{
  TimeDomain timeDomain;

  if(reader->hasAttrs())
  {
    timeDomain.defaultTime = reader->getAttr("default");
  }

  std::vector<TimeInstant> timeInstant;

  while(reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "TimeDomain")))
  {
    if(boost::iequals(reader->getElementLocalName(), "TimeInstant"))
    {
      TimeInstant time;

      if(reader->hasAttrs())
      {
        time.id = reader->getAttr("gml:id");
      }

      while(reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "TimeInstant")))
      {
        if(reader->getNodeType() == te::xml::VALUE)
        {
          if(boost::iequals(reader->getElementLocalName(), "timePosition"))
            time.timePosition = reader->getElementValue();
        }
      }

      timeInstant.push_back(time);
    }
  }

  timeDomain.timeInstant = timeInstant;

  return timeDomain;
}
