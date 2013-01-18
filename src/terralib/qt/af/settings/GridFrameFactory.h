/*  Copyright (C) 2011-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/af/settings/GridFrameFactory.h

  \brief A factory to build the grid frame object.
*/

#ifndef __TERRALIB_QT_AF_GRIDFRAMEFACTORY_H
#define __TERRALIB_QT_AF_GRIDFRAMEFACTORY_H

// QT
#include <QtGui/QWidget>

// TerraLib
#include "../../../common/AbstractFactory.h"
#include "../Config.h"
#include "ComponentWidgetFactory.h"
#include "GridFrame.h"

namespace te
{
  namespace qt
  {   
    namespace af
    {
      /*!
        \class GridFrameFactory

        \brief A factory to build the grid frame object.
      */
      class TEQTAFEXPORT GridFrameFactory : public te::qt::af::ComponentWidgetFactory
      {
        public:

          static void initialize();

          static void finalize();

          const std::string& getType() const;

          ~GridFrameFactory();

        protected:        

          te::qt::af::ComponentWidget* build();

          GridFrameFactory();

          virtual te::qt::af::ComponentWidget* create(QWidget* parent = 0);

        private:

          static GridFrameFactory* sm_factory;
      };
    } // end namespace af
  }   // end namespace qt
}     // end namespace te
#endif // __TERRALIB_QT_AF_GRIDFRAMEFACTORY_H