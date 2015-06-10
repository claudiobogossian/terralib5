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

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib.h

  \brief This file contains include headers for all modules from TerraLib that were built.
 */

#ifndef __TERRALIB_INTERNAL_TERRALIB_H__
#define __TERRALIB_INTERNAL_TERRALIB_H__


#include "terralib/Config.h"

#include "terralib/common.h"

#ifdef TERRALIB_MOD_ANNOTATIONTEXT_ENABLED
#include "terralib/annotationtext.h"
#endif

#ifdef TERRALIB_MOD_COLOR_ENABLED
#include "terralib/color.h"
#endif

#ifdef TERRALIB_MOD_DATAACCESS_ENABLED
#include "terralib/dataaccess.h"
#endif

#ifdef TERRALIB_MOD_DATATYPE_ENABLED
#include "terralib/datatype.h"
#endif

#ifdef TERRALIB_MOD_FILTER_ENCODING_ENABLED
#include "terralib/fe.h"
#endif

#ifdef TERRALIB_MOD_GEOMETRY_ENABLED
#include "terralib/geometry.h"
#endif

#ifdef TERRALIB_MOD_GML_ENABLED
#include "terralib/gml.h"
#endif

#ifdef TERRALIB_MOD_GRAPH_ENABLED
#include "terralib/graph.h"
#endif

#ifdef TERRALIB_MOD_MAPTOOLS_ENABLED
#include "terralib/maptools.h"
#endif

#ifdef TERRALIB_MOD_MEMORY_ENABLED
#include "terralib/memory.h"
#endif

#ifdef TERRALIB_MOD_METADATA_ENABLED
#include "terralib/metadata.h"
#endif

#ifdef TERRALIB_MOD_PLUGIN_ENABLED
#include "terralib/plugin.h"
#endif

#ifdef TERRALIB_MOD_QT_ENABLED
#include "terralib/qt.h"
#endif

#ifdef TERRALIB_MOD_RASTER_ENABLED
#include "terralib/raster.h"
#endif

#ifdef TERRALIB_MOD_RP_ENABLED
#include "terralib/rp.h"
#endif

#ifdef TERRALIB_MOD_SA_CORE_ENABLED
#include "terralib/sa.h"
#endif

#ifdef TERRALIB_MOD_SAM_ENABLED
#include "terralib/sam.h"
#endif

#ifdef TERRALIB_MOD_SE_ENABLED
#include "terralib/se.h"
#endif

#ifdef TERRALIB_MOD_SRS_ENABLED
#include "terralib/srs.h"
#endif

#ifdef TERRALIB_MOD_ST_ENABLED
#include "terralib/st.h"
#endif

#ifdef TERRALIB_MOD_STATISTICS_ENABLED
#include "terralib/statistics.h"
#endif

#ifdef TERRALIB_MOD_XLINK_ENABLED
#include "terralib/xlink.h"
#endif

#endif  // __TERRALIB_INTERNAL_TERRALIB_H__
