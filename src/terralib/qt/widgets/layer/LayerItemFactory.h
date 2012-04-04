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
  \file LayerItemFactory.h

  \brief This is the concrete factory for LayerItem objects.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_LAYERITEMFACTORY_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_LAYERITEMFACTORY_H

// TerraLib
#include "AbstractLayerItemFactory.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class LayerItemFactory

        \brief This is the concrete factory for LayerItem objects.

        \sa AbstractLayerItemFactory, LayerItemFactory
       */
      class TEQTWIDGETSEXPORT LayerItemFactory : public AbstractLayerItemFactory
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
          ~LayerItemFactory();

          //@}

          /** @name Public Initializer
           *  Methods for the control of the factory singleton.
           */
          //@{

          /*! \brief It initializes the factory: the singleton instance will be registered in the abstract factory AbstractLayerItemFactory. */
          static void initialize();

          /*! \brief It finalizes the factory: the singleton instance will be destroyed and will be unregistered from the abstract factory AbstractLayerItemFactory. */
          static void finalize();

          //@}

        protected:        

          /** @name Re-implementation of Pure Virtual Method
           *  Concrete factory methods.
           */
          //@{

          /*!
            \brief It creates a new LayerItem.

            \param p The parameter to be passed to the object being created.

            \return It returns an object created by the concrete factory.
           */
          AbstractLayerItem* build(std::pair<te::map::AbstractLayer*, te::qt::widgets::AbstractLayerItem*> p);

          //@}

          /** @name Protected Initializers
           *  Initializers.
           */
          //@{

          /*! \brief It initializes the factory. */
          LayerItemFactory();

          //@}

        private:

          static LayerItemFactory* sm_factory; //!< A pointer to the global LayerItem factory.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_LAYERITEMFACTORY_H


