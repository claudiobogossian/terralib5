// Example
#include "ReaderExample.h"

// TerraLib
#include <terralib/xml.h>

// STL
#include <iostream>
#include <memory>

void NodeTypePrinter(te::xml::NodeType type)
{
  switch(type)
  {
    case te::xml::UNKNOWN:
      std::cout << "UNKNOWN";
    break;

    case te::xml::START_DOCUMENT:
      std::cout << "START_DOCUMENT";
    break;

    case te::xml::END_DOCUMENT:
      std::cout << "END_DOCUMENT";
    break;

    case te::xml::START_ELEMENT:
      std::cout << "START_ELEMENT";
    break;

    case te::xml::END_ELEMENT:
      std::cout << "END_ELEMENT";
    break;

    case te::xml::VALUE:
      std::cout << "VALUE";
    break;

    case te::xml::CDATA:
      std::cout << "CDATA";
    break;

    case te::xml::IGNORABLE_WHITESPACE:
      std::cout << "IGNORABLE_WHITESPACE";
    break;

    case te::xml::WHITESPACE:
      std::cout << "WHITESPACE";
    break;

    case te::xml::PROCESSING_INSTRUCTION:
      std::cout << "PROCESSING_INSTRUCTION";
    break;

    case te::xml::START_PREFIX_MAPPING:
      std::cout << "START_PREFIX_MAPPING";
    break;

    case te::xml::END_PREFIX_MAPPING:
      std::cout << "END_PREFIX_MAPPING";
    break;

    case te::xml::SKIPPED_ENTITY:
      std::cout << "SKIPPED_ENTITY";
    break;
  }
  std::cout << std::endl;
}

void ReadXML(const std::string& path, bool validSchema, bool ignoreWhiteSpaces)
{
  try
  {
    std::auto_ptr<te::xml::Reader> reader(te::xml::ReaderFactory::make("XERCES"));
    reader->setValidationScheme(validSchema);
    reader->setIgnoreWhiteSpaces(ignoreWhiteSpaces);

    reader->read(path);

    std::cout << ":: File information: " << path;
    std::cout << std::endl;

    while(reader->next())
    {
      std::cout << "Element Name: " << reader->getElementLocalName() << " -> ";
      std::cout << "Node Type: "; NodeTypePrinter(reader->getNodeType());

      if(reader->getNodeType() == te::xml::START_ELEMENT && reader->hasAttrs())
      {
        std::cout << "  Attributes" << std::endl;
        for(std::size_t i = 0; i < reader->getNumberOfAttrs(); ++i)
          std::cout << "  * [" << reader->getAttrLocalName(i) << " = " << reader->getAttr(i) << "]" << std::endl;
      }

      if(reader->getNodeType() == te::xml::VALUE)
        std::cout << "  * Element value: " << reader->getElementValue() << std::endl;
    }
    std::cout << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred: " << e.what() << std::endl;
    return;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred!" << std::endl;
    return;
  }
}
