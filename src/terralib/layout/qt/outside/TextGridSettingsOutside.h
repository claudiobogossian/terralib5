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
  \file TextGridSettingsOutside.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEXT_GRID_SETTINGS_OUTSIDE_H
#define __TERRALIB_LAYOUT_INTERNAL_TEXT_GRID_SETTINGS_OUTSIDE_H

// Qt
#include <QDialog>

// TerraLib
#include "../../core/pattern/mvc/AbstractOutsideView.h"
#include "../../core/Config.h"
#include "../../../geometry/Envelope.h"
#include "../../../color/RGBAColor.h"
#include "../../core/enum/AbstractType.h"

namespace Ui { class TextGridSettings; }

namespace te
{
  namespace layout
  {
    class AbstractOutsideController;
    class TextGridSettingsConfigProperties;

    class TELAYOUTEXPORT TextGridSettingsOutside : public QDialog, public AbstractOutsideView
    {
      Q_OBJECT
    
      public:
        
        TextGridSettingsOutside(AbstractOutsideController* controller);

        virtual ~TextGridSettingsOutside();
        
        virtual void setPosition(const double& x, const double& y);

        virtual te::gm::Coord2D getPosition();

        virtual void blockComponents();

        virtual void unblockComponents();
       
        virtual void load();

        virtual bool  eventFilter ( QObject * watched, QEvent * event );

      signals:

        void updateProperty();

      private slots:
        
        virtual void on_pbApply_clicked();

        virtual void on_pbCancel_clicked();

        virtual void on_cmbColumnsWidthConstraints_currentIndexChanged ( const QString & text );

        virtual void on_cmbTableWidthConstraints_currentIndexChanged ( const QString & text );

        virtual void on_cmbHeaderAlign_currentIndexChanged ( const QString & text );

        virtual void on_cmbBorderStyle_currentIndexChanged ( const QString & text );

        virtual void on_cmbCellAlign_currentIndexChanged ( const QString & text );

        virtual void on_lneEdtBorderWidth_editingFinished();

        virtual void on_lneEdtColumnsNumber_editingFinished();

        virtual void on_lneEdtColumnsWidth_editingFinished();
        
        virtual void on_lneEdtRowsNumber_editingFinished();

        virtual void on_lneEdtTablePadding_editingFinished();

        virtual void on_lneEdtTableSpacing_editingFinished();

        virtual void on_lneEdtTableWidth_editingFinished();

      protected:

        virtual te::color::RGBAColor configColor(QWidget* widget);

        virtual void init();

        virtual void initString( QWidget* widget, std::string nameComponent );

        virtual void initInt( QWidget* widget, std::string nameComponent );

        virtual void initDouble( QWidget* widget, std::string nameComponent );

        virtual void initBool( QWidget* widget, std::string nameComponent );

        virtual void initColor( QWidget* widget, std::string nameComponent );

        virtual void initCombo(QWidget* widget, std::string nameComponent);

      protected:

        TextGridSettingsConfigProperties* m_propertiesNames;

      private:

        std::auto_ptr<Ui::TextGridSettings> m_ui;
    };
  }
}

#endif
