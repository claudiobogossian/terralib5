/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/mnt/core/Profile.h

  \brief This file contains a class that represents the profile.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_MNT_INTERNAL_PROFILE_H
#define __TERRALIB_MNT_INTERNAL_PROFILE_H

// Terralib Includes
#include "Config.h"
//#include "CreateIsolinesParams.h"

#include "../../../../src/terralib/dataaccess.h"
#include "../../../../src/terralib/dataaccess/datasource/DataSourceFactory.h"
#include "../../../../src/terralib/memory/DataSet.h"
#include "../../../../src/terralib/common/Holder.h"
#include "../../../../src/terralib/statistics/core/Enums.h"

// STL Includes
#include <map>
#include <memory>


namespace te
{
  //forward declarations
  namespace da  { class DataSetType; }
  namespace gm  { class Geometry; }
  namespace mem { class DataSet; }
}


namespace te
{
  namespace mnt
  {
    class TEMNTEXPORT Profile
    {
      /*!
      \class CreateIsolines

      \brief Class used to execute the create isolines

      */

    public:

      /*! \brief Default constructor. */
      Profile();

      /*! \brief Virtual destructor. */
      ~Profile();

    public:

      bool runRasterProfile(std::auto_ptr<te::rst::Raster> raster, std::vector<te::gm::LineString*> visadas, std::vector< std::vector<te::gm::LineString*> >& profileSet);
      std::vector<te::gm::LineString*> prepareVector(std::string &inDsetName, te::da::DataSourcePtr &inDsrc, std::string &geostype);
    
      std::auto_ptr<te::mem::DataSet> createDataSet(te::da::DataSet* inputDataSet, te::da::DataSetType* dsType);
      std::auto_ptr<te::rst::Raster> getPrepareRaster();
      void setInput(te::da::DataSourcePtr inRasterDsrc, std::string inRasterName, std::auto_ptr<te::da::DataSetType> inDsetType);
      int m_srid;
      te::da::DataSourcePtr m_inRasterDsrc;
      std::string m_inRasterName;
      std::auto_ptr<te::da::DataSetType> m_inRasterDsType;

  
    };
  }
}

#endif //__TERRALIB_MNT_INTERNAL_CREATEISOLINES_H