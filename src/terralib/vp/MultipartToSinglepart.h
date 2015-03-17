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
  \file MultipartToSinglepart.h
   
  \brief 
 
  \ingroup vp
*/

#ifndef __TERRALIB_VP_INTERNAL_MULTIPARTTOSINGLEPART_H
#define __TERRALIB_VP_INTERNAL_MULTIPARTTOSINGLEPART_H

//Terralib
#include "../datatype/Property.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../geometry/Geometry.h"
#include "../memory/DataSet.h"
#include "../statistics/core/Enums.h"
#include "Config.h"
#include "Enums.h"
#include "Exception.h"

// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace te
{
  namespace gm
  {
    class Geometry;
  }

  namespace vp
  {
    class TEVPEXPORT MultipartToSinglepart
    {

      public:

        MultipartToSinglepart();

        ~MultipartToSinglepart();

        bool run();

        void setInput(te::da::DataSourcePtr inDsrc,
                      std::string inDsName,
                      std::auto_ptr<te::da::DataSetType> inDsType,
                      const te::da::ObjectIdSet* oidSet = 0);

        void setOutput(te::da::DataSourcePtr outDsrc, std::string outDsName);

      protected:

        std::auto_ptr<te::da::DataSetType> getOutDst(bool onlySelected = false);

        te::gm::Geometry* single2multi(te::gm::Geometry* geom);

        bool runAll();

        bool runSelected();

      private:

        te::da::DataSourcePtr m_inDsrc;
        std::string m_inDsName;
        std::auto_ptr<te::da::DataSetType> m_inDsType;
        const te::da::ObjectIdSet* m_oidSet;
      
        te::da::DataSourcePtr m_outDsrc;
        std::string m_outDsName;
    }; // end class
  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_MULTIPARTTOSINGLEPART_H

