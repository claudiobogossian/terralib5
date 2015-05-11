/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file metadata.h

  \brief This file contains include headers for the metadata module of TerraLib.
*/

#ifndef __TERRALIB_INTERNAL_METADATA_H
#define __TERRALIB_INTERNAL_METADATA_H


// metadata
#include "metadata/Config.h"
#include "metadata/MetadataAnnotation.h"
#include "metadata/MetadataManager.h"
#include "metadata/MetadataExtractor.h"
#include "metadata/Module.h"
#include "metadata/iso19115/CI_Citation.h"
#include "metadata/iso19115/CI_ResponsibleParty.h"
#include "metadata/iso19115/CodeList.h"
#include "metadata/iso19115/MD_DataIdentification.h"
#include "metadata/iso19115/MD_Distribution.h"
#include "metadata/iso19115/MD_Format.h"
#include "metadata/iso19115/MD_Identification.h"
#include "metadata/iso19115/MD_Metadata.h"

namespace te
{
  /*!
    \brief Namespace for the memory data source of TerraLib.
  */
  namespace md
  {
  }	// end namespace mem
}   // end namespace te

#endif  // __TERRALIB_INTERNAL_MEMORY_H

