/*  Copyright(C) 2008 National Institute For Space Research(INPE) - Brazil.

This file is part of the TerraLib - a Framework for building GIS enabled applications.

TerraLib is free software : you can redistribute it and / or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License,
or(at your option) any later version.

TerraLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with TerraLib.See COPYING.If not, write to
TerraLib Team at <terralib - team@terralib.org>.
*/

/*!
\file terralib/mnt/qt/ProfielResultDialog.cpp

\brief A dialog Show Profile Graphic Result
*/

#include "../../qt/widgets/charts/ChartDisplay.h"
#include "../../qt/widgets/charts/ChartDisplayWidget.h"
#include "../../qt/widgets/charts/ChartStyle.h"

#include "ProfileResultDialog.h"
#include "ui_ProfileResultDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

#include <fstream>

//QWT
#include <qwt_symbol.h>
#include <qwt_text.h>
#include <qwt_plot_curve.h>

void GeraTeste(QwtPlotCurve *profile, QColor cor)
{
  profile->setOrientation(Qt::Horizontal);

  QVector<QPointF> values;
  for (int i = 0; i < 30; ++i)
  {
    values.push_back(QPointF(i, rand()+cor.red()));
  }

  profile->setSamples(values);

  QPen CurvePen;
  CurvePen.setColor(cor);
  CurvePen.setStyle(Qt::SolidLine);
  CurvePen.setWidth(0);
  profile->setPen(CurvePen);
}

te::mnt::ProfileResultDialog::ProfileResultDialog(QString tittle, QString yaxis, std::vector< std::vector<te::gm::LineString*> > &profileSet,
  QWidget* parent,
  Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::ProfileResultDialogForm)
{
  // add controls
  m_ui->setupUi(this);

  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_OKPushButton, SIGNAL(clicked()), this, SLOT(onOKPushButtonClicked()));

  QwtPlotCurve *profile0 = new QwtPlotCurve("Profile0");
  QwtPlotCurve *profile1 = new QwtPlotCurve("Profile1");
  QwtPlotCurve *profile2 = new QwtPlotCurve("Profile2");
  GeraTeste(profile0, QColor(Qt::green));
  GeraTeste(profile1, QColor(Qt::red));
  GeraTeste(profile2, QColor(Qt::cyan));

  //Creating and adjusting the chart Display's style.
  te::qt::widgets::ChartStyle* chartStyle = new te::qt::widgets::ChartStyle();
  chartStyle->setTitle(tittle);
  chartStyle->setAxisX(QString::fromStdString("m"));
  chartStyle->setAxisY(yaxis);
  chartStyle->setGridChecked(true);

  //Creating and adjusting the chart Display
  te::qt::widgets::ChartDisplay* chartDisplay = new te::qt::widgets::ChartDisplay(this->parentWidget(), QString::fromStdString("Profile"), chartStyle);
  chartDisplay->adjustDisplay();
  profile0->attach(chartDisplay);
  profile1->attach(chartDisplay);
  profile2->attach(chartDisplay);

  //Adjusting the chart widget
  te::qt::widgets::ChartDisplayWidget* displayWidget = new te::qt::widgets::ChartDisplayWidget(profile0, profile0->rtti(), chartDisplay, m_ui->m_profileWidget);
  displayWidget->setWindowTitle("Profile");

  QGridLayout* layout = new QGridLayout(m_ui->m_profileWidget);
  layout->addWidget(displayWidget);
  //this->layout()->setSizeConstraint(QLayout::SetFixedSize);

  displayWidget->show();
  displayWidget->setAttribute(Qt::WA_DeleteOnClose, true);
}

te::mnt::ProfileResultDialog::~ProfileResultDialog()
{

}

void te::mnt::ProfileResultDialog::onHelpPushButtonClicked()
{
}

void te::mnt::ProfileResultDialog::onOKPushButtonClicked()
{
  accept();
}

