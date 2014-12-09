/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/layer/utils/CompositionModeDialog.cpp

  \brief This file defines a class for a CompositionModeDialog.
*/

// TerraLib
#include "../../../../maptools/Enums.h"
#include "CompositionModeDialog.h"
#include "ui_CompositionModeDialogForm.h"

te::qt::widgets::CompositionModeDialog::CompositionModeDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::CompositionModeDialogForm)
{
//build form
  m_ui->setupUi(this);

  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("layer-compose-hint").pixmap(112,48));

//connects
  connect(m_ui->m_clearRadioButton, SIGNAL(clicked(bool)), this, SLOT(onClearRadioButtonClicked(bool)));
  connect(m_ui->m_srcRadioButton, SIGNAL(clicked(bool)), this, SLOT( onSourceRadioButtonClicked(bool)));
  connect(m_ui->m_dstRadioButton, SIGNAL(clicked(bool)), this, SLOT( onDestinationRadioButtonClicked(bool)));
  connect(m_ui->m_srcOverRadioButton, SIGNAL(clicked(bool)), this, SLOT( onSourceOverRadioButtonClicked(bool)));
  connect(m_ui->m_dstOverRadioButton, SIGNAL(clicked(bool)), this, SLOT( onDestinationOverRadioButtonClicked(bool)));
  connect(m_ui->m_srcInRadioButton, SIGNAL(clicked(bool)), this, SLOT( onSourceInRadioButtonClicked(bool)));
  connect(m_ui->m_dstInRadioButton, SIGNAL(clicked(bool)), this, SLOT( onDestinationInRadioButtonClicked(bool)));
  connect(m_ui->m_srcOutRadioButton, SIGNAL(clicked(bool)), this, SLOT( onSourceOutRadioButtonClicked(bool)));
  connect(m_ui->m_dstOutRadioButton, SIGNAL(clicked(bool)), this, SLOT( onDestinationOutRadioButtonClicked(bool)));
  connect(m_ui->m_srcAtTopRadioButton, SIGNAL(clicked(bool)), this, SLOT( onSourceAtTopRadioButtonClicked(bool)));
  connect(m_ui->m_dstAtTopRadioButton, SIGNAL(clicked(bool)), this, SLOT( onDestinationAtTopRadioButtonClicked(bool)));
  connect(m_ui->m_xorRadioButton, SIGNAL(clicked(bool)), this, SLOT( onXorRadioButtonClicked(bool)));
  connect(m_ui->m_plusRadioButton, SIGNAL(clicked(bool)), this, SLOT( onPlusRadioButtonClicked(bool)));
  connect(m_ui->m_multRadioButton, SIGNAL(clicked(bool)), this, SLOT( onMultiplyRadioButtonClicked(bool)));
  connect(m_ui->m_screenRadioButton, SIGNAL(clicked(bool)), this, SLOT( onScreenRadioButtonClicked(bool)));
  connect(m_ui->m_overlayRadioButton, SIGNAL(clicked(bool)), this, SLOT( onOverlayRadioButtonClicked(bool)));
  connect(m_ui->m_darkenRadioButton, SIGNAL(clicked(bool)), this, SLOT( onDarkenRadioButtonClicked(bool)));
  connect(m_ui->m_lightenRadioButton, SIGNAL(clicked(bool)), this, SLOT( onLightenRadioButtonClicked(bool)));
  connect(m_ui->m_colorDodgeRadioButton, SIGNAL(clicked(bool)), this, SLOT( onColorDodgeRadioButtonClicked(bool)));
  connect(m_ui->m_colorBurnRadioButton, SIGNAL(clicked(bool)), this, SLOT( onColorBurnRadioButtonClicked(bool)));
  connect(m_ui->m_hardLightRadioButton, SIGNAL(clicked(bool)), this, SLOT( onHardLightRadioButtonClicked(bool)));
  connect(m_ui->m_softLightRadioButton, SIGNAL(clicked(bool)), this, SLOT( onSoftLightRadioButtonClicked(bool)));
  connect(m_ui->m_diffRadioButton, SIGNAL(clicked(bool)), this, SLOT( onDifferenceRadioButtonClicked(bool)));
  connect(m_ui->m_exclusionRadioButton, SIGNAL(clicked(bool)), this, SLOT( onExclusionRadioButtonClicked(bool)));

// help info
  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins"); 
  m_ui->m_helpPushButton->setPageReference("widgets/layer/compositionMode_dialog.html");
}

te::qt::widgets::CompositionModeDialog::~CompositionModeDialog()
{
}

void te::qt::widgets::CompositionModeDialog::setLayer(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  te::map::CompositionMode cm = m_layer->getCompositionMode();

  if(cm == te::map::Clear)
  {
    m_ui->m_clearRadioButton->setChecked(true);

    onClearRadioButtonClicked(true);
  }
  else if(cm == te::map::Source)
  {
    m_ui->m_srcRadioButton->setChecked(true);

    onSourceRadioButtonClicked(true);
  }
  else if(cm == te::map::Destination)
  {
    m_ui->m_dstRadioButton->setChecked(true);

    onDestinationRadioButtonClicked(true);
  }
  else if(cm == te::map::SourceOver)
  {
    m_ui->m_srcOverRadioButton->setChecked(true);

    onSourceOverRadioButtonClicked(true);
  }
  else if(cm == te::map::DestinationOver)
  {
    m_ui->m_dstOverRadioButton->setChecked(true);

    onDestinationOverRadioButtonClicked(true);
  }
  else if(cm == te::map::SourceIn)
  {
    m_ui->m_srcInRadioButton->setChecked(true);

    onSourceInRadioButtonClicked(true);
  }
  else if(cm == te::map::DestinationIn)
  {
    m_ui->m_dstInRadioButton->setChecked(true);

    onDestinationInRadioButtonClicked(true);
  }
  else if(cm == te::map::SourceOut)
  {
    m_ui->m_srcOutRadioButton->setChecked(true);

    onSourceOutRadioButtonClicked(true);
  }
  else if(cm == te::map::DestinationOut)
  {
    m_ui->m_dstOutRadioButton->setChecked(true);

    onDestinationOutRadioButtonClicked(true);
  }
  else if(cm == te::map::SourceAtop)
  {
    m_ui->m_srcAtTopRadioButton->setChecked(true);

    onSourceAtTopRadioButtonClicked(true);
  }
  else if(cm == te::map::DestinationAtop)
  {
    m_ui->m_dstAtTopRadioButton->setChecked(true);

    onDestinationAtTopRadioButtonClicked(true);
  }
  else if(cm == te::map::Xor)
  {
    m_ui->m_xorRadioButton->setChecked(true);

    onXorRadioButtonClicked(true);
  }
  else if(cm == te::map::Plus)
  {
    m_ui->m_plusRadioButton->setChecked(true);

    onPlusRadioButtonClicked(true);
  }
  else if(cm == te::map::Multiply)
  {
    m_ui->m_multRadioButton->setChecked(true);

    onMultiplyRadioButtonClicked(true);
  }
  else if(cm == te::map::Screen)
  {
    m_ui->m_screenRadioButton->setChecked(true);

    onScreenRadioButtonClicked(true);
  }
  else if(cm == te::map::Overlay)
  {
    m_ui->m_overlayRadioButton->setChecked(true);

    onOverlayRadioButtonClicked(true);
  }
  else if(cm == te::map::Darken)
  {
    m_ui->m_darkenRadioButton->setChecked(true);

    onDarkenRadioButtonClicked(true);
  }
  else if(cm == te::map::Lighten)
  {
    m_ui->m_lightenRadioButton->setChecked(true);

    onLightenRadioButtonClicked(true);
  }
  else if(cm == te::map::ColorDodge)
  {
    m_ui->m_colorDodgeRadioButton->setChecked(true);

    onColorDodgeRadioButtonClicked(true);
  }
  else if(cm == te::map::ColorBurn)
  {
    m_ui->m_colorBurnRadioButton->setChecked(true);

    onColorBurnRadioButtonClicked(true);
  }
  else if(cm == te::map::HardLight)
  {
    m_ui->m_hardLightRadioButton->setChecked(true);

    onHardLightRadioButtonClicked(true);
  }
  else if(cm == te::map::SoftLight)
  {
    m_ui->m_softLightRadioButton->setChecked(true);

    onSoftLightRadioButtonClicked(true);
  }
  else if(cm == te::map::Difference)
  {
    m_ui->m_diffRadioButton->setChecked(true);

    onDifferenceRadioButtonClicked(true);
  }
  else if(cm == te::map::Exclusion)
  {
    m_ui->m_exclusionRadioButton->setChecked(true);

    onExclusionRadioButtonClicked(true);
  }
}

QWidget* te::qt::widgets::CompositionModeDialog::getApplyButton()
{
  return m_ui->m_applyPushButton;
}

void te::qt::widgets::CompositionModeDialog::onClearRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::Clear);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-clear").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("The pixels in the destination are cleared (set to fully transparent) independent of the source."));
  }
}

void te::qt::widgets::CompositionModeDialog::onSourceRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::Source);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-src").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("The output is the source pixel. (This means a basic copy operation and is identical to SourceOver when the source pixel is opaque)."));
  }
}

void te::qt::widgets::CompositionModeDialog::onDestinationRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::Destination);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-dst").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("The output is the destination pixel. This means that the blending has no effect. This mode is the inverse of Source."));
  }
}

void te::qt::widgets::CompositionModeDialog::onSourceOverRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::SourceOver);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-src-over").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("This is the default mode. The alpha of the source is used to blend the pixel on top of the destination."));
  }
}

void te::qt::widgets::CompositionModeDialog::onDestinationOverRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::DestinationOver);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-dst-over").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("The alpha of the destination is used to blend it on top of the source pixels. This mode is the inverse of Source Over."));
  }
}

void te::qt::widgets::CompositionModeDialog::onSourceInRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::SourceIn);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-src-in").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("The output is the source, where the alpha is reduced by that of the destination."));
  }
}

void te::qt::widgets::CompositionModeDialog::onDestinationInRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::DestinationIn);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-dst-in").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("The output is the destination, where the alpha is reduced by that of the source. This mode is the inverse of Source In."));
  }
}

void te::qt::widgets::CompositionModeDialog::onSourceOutRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::SourceOut);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-src-out").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("The output is the source, where the alpha is reduced by the inverse of destination."));
  }
}

void te::qt::widgets::CompositionModeDialog::onDestinationOutRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::DestinationOut);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-dst-out").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("The output is the destination, where the alpha is reduced by the inverse of the source. This mode is the inverse of Source Out."));
  }
}

void te::qt::widgets::CompositionModeDialog::onSourceAtTopRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::SourceAtop);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-src-atop").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("The source pixel is blended on top of the destination, with the alpha of the source pixel reduced by the alpha of the destination pixel."));
  }
}

void te::qt::widgets::CompositionModeDialog::onDestinationAtTopRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::DestinationAtop);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-dst-atop").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("The destination pixel is blended on top of the source, with the alpha of the destination pixel is reduced by the alpha of the destination pixel. This mode is the inverse of Source A top."));
  }
}

void te::qt::widgets::CompositionModeDialog::onXorRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::Xor);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-xor").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("The source, whose alpha is reduced with the inverse of the destination alpha, is merged with the destination, whose alpha is reduced by the inverse of the source alpha. Xor is not the same as the bitwise Xor."));
  }
}

void te::qt::widgets::CompositionModeDialog::onPlusRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::Plus);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-mix").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("Both the alpha and color of the source and destination pixels are added together."));
  }
}

void te::qt::widgets::CompositionModeDialog::onMultiplyRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::Multiply);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-mix").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("The output is the source color multiplied by the destination. Multiplying a color with white leaves the color unchanged, while multiplying a color with black produces black."));
  }
}

void te::qt::widgets::CompositionModeDialog::onScreenRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::Screen);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-mix").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("The source and destination colors are inverted and then multiplied. Screening a color with white produces white, whereas screening a color with black leaves the color unchanged."));
  }
}

void te::qt::widgets::CompositionModeDialog::onOverlayRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::Overlay);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-mix").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("Multiplies or screens the colors depending on the destination color. The destination color is mixed with the source color to reflect the lightness or darkness of the destination."));
  }
}

void te::qt::widgets::CompositionModeDialog::onDarkenRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::Darken);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-mix").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("The darker of the source and destination colors is selected."));
  }
}

void te::qt::widgets::CompositionModeDialog::onLightenRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::Lighten);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-mix").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("The lighter of the source and destination colors is selected."));
  }
}

void te::qt::widgets::CompositionModeDialog::onColorDodgeRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::ColorDodge);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-mix").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("The destination color is brightened to reflect the source color. A black source color leaves the destination color unchanged."));
  }
}

void te::qt::widgets::CompositionModeDialog::onColorBurnRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::ColorBurn);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-mix").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("The destination color is darkened to reflect the source color. A white source color leaves the destination color unchanged."));
  }
}

void te::qt::widgets::CompositionModeDialog::onHardLightRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::HardLight);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-mix").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("Multiplies or screens the colors depending on the source color. A light source color will lighten the destination color, whereas a dark source color will darken the destination color."));
  }
}

void te::qt::widgets::CompositionModeDialog::onSoftLightRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::SoftLight);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-mix").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("Darkens or lightens the colors depending on the source color. Similar to Hard Light."));
  }
}

void te::qt::widgets::CompositionModeDialog::onDifferenceRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::Difference);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-mix").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("Subtracts the darker of the colors from the lighter. Painting with white inverts the destination color, whereas painting with black leaves the destination color unchanged."));
  }
}

void te::qt::widgets::CompositionModeDialog::onExclusionRadioButtonClicked(bool flag)
{
  if(m_layer.get() && flag)
  {
    m_layer->setCompositionMode(te::map::Exclusion);

    m_ui->m_diagramLabel->setPixmap(QIcon::fromTheme("composition-mode-mix").pixmap(128,128));

    m_ui->m_decriptionPlainTextEdit->setPlainText(tr("Similar to Difference, but with a lower contrast. Painting with white inverts the destination color, whereas painting with black leaves the destination color unchanged."));
  }
}
