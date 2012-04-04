#include "ReaderExample.h"

#include <terralib/common.h>
#include <terralib/xml.h>

#include <iostream>
#include <string>

void Read()
{
  try {
    
    te::xml::Reader* mreader = te::xml::ReaderFactory::make("XERCES");
    if (!mreader)
      return;
    
    mreader->read("/Users/lubia/Documents/terralib5svn/examples/xerces/note.xml");
    
    if (!mreader->next())
    {
      delete mreader;
      return;
    }

    while (mreader->next())
    {
      std::cout << mreader->getElementLocalName() << " : " ;
      std::cout << mreader->getElementValue() << std::endl;
    }
    delete mreader;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried: " << e.what() << std::endl;
    return ;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried!" << std::endl;
    return ;
  }
	
}