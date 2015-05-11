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
  \file terralib/plugins/exchanger/DataExchangeStatus.h

  \brief A Qt dialog for ....
*/

#ifndef __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_DATAEXCHANGESTATUS_H
#define __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_DATAEXCHANGESTATUS_H

// TerraLib
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../Config.h"

// STL
#include <string>

// Boost
#include <boost/chrono.hpp>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      struct TEQTWIDGETSEXPORT DataExchangeStatus
      {
        te::da::DataSetType* m_dataset;
        bool m_successful;
        boost::chrono::duration<double> m_time;
        std::string m_exceptionMsg;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_DATAEXCHANGESTATUS_H
