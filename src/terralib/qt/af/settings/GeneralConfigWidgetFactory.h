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
  \file GeneralConfigWidgetFactory.h

  \brief A factory to build the general config frame object.
*/

#ifndef __TERRALIB_QT_AF_GENERALCONFIGWIDGETFACTORY_H
#define __TERRALIB_QT_AF_GENERALCONFIGWIDGETFACTORY_H

// TerraLib
#include "../Config.h"
#include "SettingsWidgetsFactory.h"

namespace te
{
  namespace qt
  {   
    namespace af
    {
      /*!
        \class GeneralConfigWidgetFactory

        \brief A factory to build the general config frame object.
      */
      class TEQTAFEXPORT GeneralConfigWidgetFactory : public SettingsWidgetsFactory
      {
        public:

          static void initialize();

          static void finalize();

          ~GeneralConfigWidgetFactory();

        protected:

          AbstractSettingWidget* build();

          GeneralConfigWidgetFactory();

          virtual AbstractSettingWidget* create(QWidget* parent = 0);

        private:

          static GeneralConfigWidgetFactory* sm_factory;
      };
    } // end namespace af
  }   // end namespace qt
}     // end namespace te
#endif // __TERRALIB_QT_AF_GENERALCONFIGWIDGETFACTORY_H