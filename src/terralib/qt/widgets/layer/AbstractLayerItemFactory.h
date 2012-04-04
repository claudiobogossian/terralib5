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
  \file AbstractLayerItemFactory.h

  \brief This is the abstract factory for Qt TreeItem objects.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_ABSTRACTLAYERITEMFACTORY_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_ABSTRACTLAYERITEMFACTORY_H

// TerraLib
#include "../../../common/ParameterizedAbstractFactory.h"
#include "../../../maptools/AbstractLayer.h"
#include "AbstractLayerItem.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class AbstractLayerItemFactory

        \brief This is the abstract factory for Qt TreeItem objects.

        \sa FeatureLayerItemFactory, FolderLayerItemFactory
       */
      class TEQTWIDGETSEXPORT AbstractLayerItemFactory : public te::common::ParameterizedAbstractFactory<AbstractLayerItem, std::string, std::pair<te::map::AbstractLayer*, te::qt::widgets::AbstractLayerItem*> >
      {
        public:

          /** @name Public Destructor
           *  Destructor.
           */
          //@{

          /*!
            \brief Virtual destructor.

            \note It will automatically unregister the factory from the dictionary.
           */
          virtual ~AbstractLayerItemFactory();

          //@} 

        protected:

          /** @name Protected Initializers
           *  Initializers.
           */
          //@{

          /*!
            \brief It creates the factory.

            The key of a AbstractLayerItemFactory is a string.

            \param factoryKey The key that identifies the factory.
           */
          AbstractLayerItemFactory(const std::string& factoryKey);        

          //@}        

      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_ABSTRACTLAYERITEMFACTORY_H


