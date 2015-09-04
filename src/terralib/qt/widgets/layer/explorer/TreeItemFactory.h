/*  Copyright (C) 2001-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file TreeItemFactory.h

  \brief A class for building layer items.
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_TREEITEMFACTORY_H
#define __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_TREEITEMFACTORY_H

// TerraLib
#include "../../../../common/Static.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../Config.h"

// STL
#include <map>
#include <string>

// Boost
#include <boost/function.hpp>

// Forward declaration
class QObject;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class TreeItem;

      /*!
        \class TreeItemFactory

        \brief A class for building layer items.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT TreeItemFactory : public te::common::Static
      {
        public:

          typedef boost::function<TreeItem* (const te::map::AbstractLayerPtr&)> FactoryFnctType;

          static TreeItem* make(const te::map::AbstractLayerPtr& layer);

          static void reg(const std::string& key, const FactoryFnctType& f);

          static void initialize();

          static void finalize();

        private:

          static std::map<std::string, FactoryFnctType> sm_factories;
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_TREEITEMFACTORY_H
