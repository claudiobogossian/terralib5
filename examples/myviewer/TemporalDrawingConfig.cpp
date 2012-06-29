#include "TemporalDrawingConfig.h"

//QT
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QAction>

TemporalDrawingConfig::TemporalDrawingConfig(QWidget* parent, Qt::WindowFlags f) : 
  QDialog(parent, f)
{
  setWindowTitle("Temporal Drawing Config");
  QVBoxLayout* vlayout = new QVBoxLayout(this);
  QLabel* label1 = new QLabel("Date Interval:", this);
  m_intervalDateComboBox = new QComboBox(this);
  QHBoxLayout* layout1 = new QHBoxLayout(this);
  layout1->addWidget(label1);
  layout1->addWidget(m_intervalDateComboBox);
  vlayout->addLayout(layout1);

  QLabel* label2 = new QLabel("Drawing Interval:", this);
  m_intervalDrawingComboBox = new QComboBox(this);
  QHBoxLayout* layout2 = new QHBoxLayout(this);
  layout2->addWidget(label2);
  layout2->addWidget(m_intervalDrawingComboBox);
  vlayout->addLayout(layout2);

  m_drawLinesCheckBox = new QCheckBox("Draw Lines", this);
  vlayout->addWidget(m_drawLinesCheckBox);

  m_loopCheckBox = new QCheckBox("Loop", this);
  vlayout->addWidget(m_loopCheckBox);

  QGroupBox* gb = new QGroupBox(this);
  m_okPushButton = new QPushButton("Ok", gb);
  m_cancelPushButton = new QPushButton("Cancel", gb);
  QHBoxLayout* layout4 = new QHBoxLayout(gb);
  layout4->addWidget(m_okPushButton);
  layout4->addWidget(m_cancelPushButton);
  vlayout->addWidget(gb);

  setLayout(vlayout);

  QObject::connect(m_okPushButton, SIGNAL(clicked()), this, SLOT(okSlot()));
  QObject::connect(m_cancelPushButton, SIGNAL(clicked()), this, SLOT(cancelSlot()));

  populeWidgets();
}

TemporalDrawingConfig::~TemporalDrawingConfig()
{
}

void TemporalDrawingConfig::populeWidgets()
{
  m_intervalDateComboBox->addItem("15 min");
  m_intervalDateComboBox->addItem("30 min");
  m_intervalDateComboBox->addItem("1 hora");
  m_intervalDateComboBox->addItem("1 dia");
  m_intervalDateComboBox->addItem("2 dias");
  m_intervalDateComboBox->addItem("3 dias");
  m_intervalDateComboBox->addItem("4 dias");
  m_intervalDateComboBox->addItem("5 dias");
  m_intervalDateComboBox->addItem("6 dias");
  m_intervalDateComboBox->addItem("7 dias");
  m_intervalDateComboBox->addItem("8 dias");
  m_intervalDateComboBox->addItem("9 dias");
  m_intervalDateComboBox->addItem("10 dias");
  m_intervalDateComboBox->addItem("20 dias");
  m_intervalDateComboBox->addItem("30 dias");
  m_intervalDateComboBox->addItem("40 dias");
  m_intervalDateComboBox->addItem("50 dias");
  m_intervalDateComboBox->addItem("60 dias");
  m_intervalDateComboBox->addItem("70 dias");
  m_intervalDateComboBox->addItem("80 dias");
  m_intervalDateComboBox->addItem("90 dias");
  m_intervalDateComboBox->addItem("100 dias");

  m_intervalDrawingComboBox->addItem("100 ms");
  m_intervalDrawingComboBox->addItem("200 ms");
  m_intervalDrawingComboBox->addItem("300 ms");
  m_intervalDrawingComboBox->addItem("400 ms");
  m_intervalDrawingComboBox->addItem("500 ms");
  m_intervalDrawingComboBox->addItem("600 ms");
  m_intervalDrawingComboBox->addItem("700 ms");
  m_intervalDrawingComboBox->addItem("800 ms");
  m_intervalDrawingComboBox->addItem("900 ms");
  m_intervalDrawingComboBox->addItem("1000 ms");
  m_intervalDrawingComboBox->addItem("1100 ms");
  m_intervalDrawingComboBox->addItem("1200 ms");
  m_intervalDrawingComboBox->addItem("1300 ms");
  m_intervalDrawingComboBox->addItem("1400 ms");
  m_intervalDrawingComboBox->addItem("1500 ms");
  m_intervalDrawingComboBox->addItem("1600 ms");
  m_intervalDrawingComboBox->addItem("1700 ms");
  m_intervalDrawingComboBox->addItem("1800 ms");
  m_intervalDrawingComboBox->addItem("1900 ms");
  m_intervalDrawingComboBox->addItem("2000 ms");
}

void TemporalDrawingConfig::okSlot()
{
  accept();
  hide();
}

void TemporalDrawingConfig::cancelSlot()
{
  reject();
  hide();
}
