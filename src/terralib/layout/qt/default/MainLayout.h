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
 \file MainLayout.h
 
 \brief A class 
 */

#ifndef __TE_QT_PLUGINS_LAYOUT_INTERNAL_MAINLAYOUT_H
#define __TE_QT_PLUGINS_LAYOUT_INTERNAL_MAINLAYOUT_H

// TerraLib
#include "../../core/Config.h"
#include "../../../layout/qt/core/View.h"
#include "BuildContext.h"
#include "BuildEnums.h"

// Qt
#include <QStatusBar>
#include <QGroupBox>
#include <QMenu>
#include <QWidget>
#include <QObject>

namespace te
{
  namespace layout
  {
    class DisplayDock;
    class OutsideArea;
    class AbstractProxyProject;
    class PropertiesDock;
    class ObjectInspectorDock;
    class ToolbarOutside;
    class EditTemplateDock;


    /*!
    \class

    \brief A class ...within TerraLib environment.

    */
    class TELAYOUTEXPORT MainLayout : public QObject
    {
      Q_OBJECT //for slots/signals

      public:

        /*! \brief Constructor. */
        MainLayout(AbstractProxyProject* proxyProject);

        //! Destructor.
        ~MainLayout();

        /*!
        \brief Inializes

        This methods
        \exception te::layout::Exception if the
        */
        virtual void init(const QSize& size, const QRect& screen);

        virtual QStatusBar* getStatusBar();

        virtual te::layout::View* getView();

        virtual te::layout::PropertiesDock* getProperties();

        virtual te::layout::ObjectInspectorDock* getObjectInspector();

        virtual te::layout::ToolbarOutside* getToolbar();

        virtual te::layout::EditTemplateDock* getEditTemplate();

        virtual QMenu* getMenu(QMenu* parentMenu);

        virtual void postInit();

      protected:



/*        void createDockLayoutDisplay(te::layout::View* view);*/

        void createLayoutContext(int width, int height);

        void createEnums();

        //void finish();

        te::layout::View*         m_view;
        QStatusBar*               m_statusBar;
        te::layout::BuildContext* m_buildContext;
        OutsideArea*              m_outsideArea;
        te::layout::BuildEnums*   m_buildEnums;
        AbstractProxyProject*             m_proxyProject;

      signals:

        void exit();

      public slots:

        void onExit();


      private:

        /*!
        \brief Copy constructor not allowed.

        \param rhs The right-hand-side copy that would be used to copy from.
        */
        MainLayout(const MainLayout& rhs);

        /*!
        \brief Assignment operator not allowed.

        \param rhs The right-hand-side copy that would be used to copy from.

        \return A reference to this object.
        */
        MainLayout& operator=(const MainLayout& rhs);
    };
  }
}
#endif // __TERRALIB_LAYOUT_INTERNAL_MAINLAYOUT_H

