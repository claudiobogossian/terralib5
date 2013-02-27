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
  \file terralib/qt/af/settings/TableFrameFactory.h

  \brief A factory to build the Table frame object.
*/

#ifndef __TERRALIB_QT_AF_TABLEFRAMEFACTORY_H
#define __TERRALIB_QT_AF_TABLEFRAMEFACTORY_H

// QT
#include <QtGui/QWidget>

// TerraLib
#include "../../../common/AbstractFactory.h"
#include "../Config.h"
#include "ComponentWidgetFactory.h"
#include "TableFrame.h"

namespace te
{
  namespace qt
  {   
    namespace af
    {
      /*!
        \class TableFrameFactory

        \brief A factory to build the Table frame object.
      */
      class TEQTAFEXPORT TableFrameFactory : public te::qt::af::ComponentWidgetFactory
      {
        public:

          static void initialize();

          static void finalize();

          const std::string& getType() const;

          ~TableFrameFactory();

        protected:        

          te::qt::af::ComponentWidget* build();

          TableFrameFactory();

          virtual te::qt::af::ComponentWidget* create(QWidget* parent = 0);

        private:

          static TableFrameFactory* sm_factory;
      };
    } // end namespace af
  }   // end namespace qt
}     // end namespace te
#endif // __TERRALIB_QT_AF_TABLEFRAMEFACTORY_H