///*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

//    This file is part of the TerraLib - a Framework for building GIS enabled applications.

//    TerraLib is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License,
//    or (at your option) any later version.

//    TerraLib is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU Lesser General Public License for more details.

//    You should have received a copy of the GNU Lesser General Public License
//    along with TerraLib. See COPYING. If not, write to
//    TerraLib Team at <terralib-team@terralib.org>.
// */

///*!
//  \file terralib/qt/af/ProjectInfoWidget.h

//  \brief Contains a widget for define Project informations.
//*/

//#ifndef __TERRALIB_QT_AF_INTERNAL_PROJECTINFOWIDGET_H
//#define __TERRALIB_QT_AF_INTERNAL_PROJECTINFOWIDGET_H

//#include <QWidget>

//#include "Config.h"

//// Forward declaration
//namespace Ui
//{
//  class ProjectInfoWidgetForm;
//}

//namespace te
//{
//  namespace qt
//  {
//    namespace af
//    {
//      // Forward declaration
//      class Project;

//      /*!
//        \class ProjectInfoWidget

//        \brief A widget for present te::qt::af::Project informations.
//      */
//      class TEQTAFEXPORT ProjectInfoWidget : public QWidget
//      {
//        public:

//          /*!
//            \brief Constructor.

//            \param parent QWidget parent.
//          */
//          ProjectInfoWidget(QWidget* parent=0);

//          /*!
//            \brief Destructor.
//          */
//          ~ProjectInfoWidget();

//          /*!
//            \brief Updates the te::qt::af::Project being visualized.

//            \param proj The te::qt::af::Project to be presented.
//          */
//          void setProject(Project* proj);

//          /*!
//            \brief Updates the te::qt::af::Project informations.
//          */
//          void updateProjectInfo();

//        protected:

//          Project* m_proj;      //!< te::qt::af::Project being presented.

//        private:

//          Ui::ProjectInfoWidgetForm* m_ui;  //!< Pointer to the form.
//      };
//    }
//  }
//}

//#endif //__TERRALIB_QT_AF_INTERNAL_PROJECTINFOWIDGET_H
