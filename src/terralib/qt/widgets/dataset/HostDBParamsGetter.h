/*  Copyright (C) 2001-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file HostDBParamsGetter.h
  \brief 
  \details 
  \version 5.0
  \author Frederico Augusto Bed&ecirc; &lt;frederico.bede@funcate.org.br&gt;
  \date 2001-2012
  \copyright GNU Lesser General Public License.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_HOSTDBPARAMSGETTER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_HOSTDBPARAMSGETTER_H

//! Terralib include files
#include <terralib/qt/widgets/Config.h>
#include <terralib/maptools/DSParamsGetter.h>

//! Qt include files
#include <QWidget>

//! Forward declarations
namespace Ui
{
  class HostDBParamsGetter;
}

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT HostDBParamsGetter : public QWidget, public te::map::DSParamsGetter
      {
      public:
        
        HostDBParamsGetter(QWidget* parent=0);

        HostDBParamsGetter(QWidget* parent, const std::map<std::string, std::string>& info);

        std::map<std::string, std::string> getParams() const;

      protected:

        void init(const std::map<std::string, std::string>& info);

      private:

        Ui::HostDBParamsGetter* m_ui;
      };
    }
  }
}

#endif //! __TERRALIB_QT_WIDGETS_INTERNAL_HOSTDBPARAMSGETTER_H
