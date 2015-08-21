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
  \file PageSetupOutside.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PAGE_SETUP_OUTSIDE_H
#define __TERRALIB_LAYOUT_INTERNAL_PAGE_SETUP_OUTSIDE_H

// Qt
#include <QDialog>

// TerraLib
#include "../../core/pattern/mvc/AbstractOutsideView.h"
#include "../../core/Config.h"
#include "../../../geometry/Envelope.h"
#include "../../../color/RGBAColor.h"
#include "../../core/enum/AbstractType.h"

namespace Ui { class PageSetup; }

namespace te
{
  namespace layout
  {
    class AbstractOutsideController;
    class Scene;

    class TELAYOUTEXPORT PageSetupOutside : public QDialog, public AbstractOutsideView
    {
      Q_OBJECT
    
      public:
        
        PageSetupOutside(AbstractOutsideController* controller);

        virtual ~PageSetupOutside();
        
        virtual void setPosition(const double& x, const double& y);

        virtual te::gm::Coord2D getPosition();
       
        virtual void load();

      signals:

        void changeConfig();

      private slots:

        virtual void on_cmbPageSize_currentIndexChanged ( const QString & text );

        virtual void on_pbApply_clicked();

        virtual void on_rdbLandscape_clicked();

        virtual void on_rdbPortrait_clicked();

      protected:

        virtual void init();

        /** \brief Configures the orientation page getting from layout 
        */
        virtual void configureOrientationPage();

        /** \brief Configures the page size getting from layout
        */
        virtual void configurePageSize();

        virtual void switchSize();

        Scene* getScene();

        te::layout::LayoutOrientationType m_orientation;
        te::layout::LayoutAbstractPaperType m_paperType;
                        
      private:

        std::auto_ptr<Ui::PageSetup> m_ui;
    };
  }
}

#endif
