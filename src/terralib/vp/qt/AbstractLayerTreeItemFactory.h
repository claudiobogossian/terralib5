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
  \file terralib/qt/widgets/layer/explorer/AbstractLayerTreeItemFactory.h

  \brief A class for building layer items. 
*/

#ifndef __TERRALIB_VP_QT_INTERNAL_ABSTRACTLAYERTREEITEMFACTORY_H
#define __TERRALIB_VP_QT_INTERNAL_ABSTRACTLAYERTREEITEMFACTORY_H

// TerraLib
#include "../../common/Static.h"
#include "../../maptools/AbstractLayer.h"
#include "../core/Config.h"

// STL
#include <map>
#include <string>

// Boost
#include <boost/function.hpp>

// Forward declaration
class QObject;

namespace te
{
  namespace vp
  {
    class AbstractLayerTreeItem;

    class TEVPEXPORT AbstractLayerTreeItemFactory : public te::common::Static
    {
      public:

        typedef boost::function<AbstractLayerTreeItem* (const te::map::AbstractLayerPtr&, QObject*)> FactoryFnctType;

        static AbstractLayerTreeItem* make(const te::map::AbstractLayerPtr& layer, QObject* parent);

        static void initialize();

        static void finalize();

      private:

        static std::map<std::string, FactoryFnctType> sm_factories;
    };

    inline AbstractLayerTreeItem* AbstractLayerTreeItemFactory::make(const te::map::AbstractLayerPtr& layer, QObject* parent)
    {
      return sm_factories[layer->getType()](layer, parent);
    }
  }   // end namespace vp
}     // end namespace te

#endif  // __TERRALIB_VP_QT_INTERNAL_ABSTRACTLAYERTREEITEMFACTORY_H

