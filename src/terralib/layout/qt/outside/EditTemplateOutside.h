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

#ifndef __TERRALIB_LAYOUT_INTERNAL_EDIT_TEMPLATE_OUTSIDE_H
#define __TERRALIB_LAYOUT_INTERNAL_EDIT_TEMPLATE_OUTSIDE_H

// Qt
#include <QWidget>

// TerraLib
#include "../../core/pattern/mvc/AbstractOutsideView.h"
#include "../../core/Config.h"
#include "terralib/geometry/Coord2D.h"

namespace Ui { class EditTemplate; }

namespace te
{
  namespace layout
  {
		class AbstractOutsideController;

		class TELAYOUTEXPORT EditTemplateOutside : public QWidget, public AbstractOutsideView
    {
	    Q_OBJECT
    
      public:
        
				EditTemplateOutside(AbstractOutsideController* controller);

		    virtual ~EditTemplateOutside();
				
        virtual void setPosition(const double& x, const double& y);

        virtual te::gm::Coord2D getPosition();
       
        virtual void load();

      signals:

        void changeTemplate();

      private slots:
        
        virtual void on_pbApply_clicked();
        
      protected:

        virtual void init();

      private:

        std::auto_ptr<Ui::EditTemplate> m_ui;
    };
  }
}

#endif
