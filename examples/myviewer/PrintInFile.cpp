#include "PrintInFile.h"
#include <QMessageBox>
#include <QPrinter>

PrintInFile::PrintInFile(QWidget* parent) : QDialog(parent)
{
  QGroupBox* gb1 = new QGroupBox("File Format", this);
  QHBoxLayout* h1 = new QHBoxLayout(gb1);
  m_fileFormatComboBox = new QComboBox(gb1);
  m_fileFormatComboBox->addItem("PDF");
  m_fileFormatComboBox->addItem("Post Script");
  h1->addWidget(m_fileFormatComboBox);

  QGroupBox* gb2 = new QGroupBox("Paper Size", this);
  QGroupBox* gb2a = new QGroupBox("", gb2);
  QHBoxLayout* h2a = new QHBoxLayout(gb2a);
  m_paperSizeComboBox = new QComboBox(gb2a);
  m_paperSizeComboBox->addItem("A0");
  m_paperSizeComboBox->addItem("A1");
  m_paperSizeComboBox->addItem("A2");
  m_paperSizeComboBox->addItem("A3");
  m_paperSizeComboBox->addItem("A4");
  m_paperSizeComboBox->addItem("A5");
  m_paperSizeComboBox->addItem("A6");
  m_paperSizeComboBox->addItem("A7");
  m_paperSizeComboBox->addItem("A8");
  m_paperSizeComboBox->addItem("A9");
  m_paperSizeComboBox->addItem("B0");
  m_paperSizeComboBox->addItem("B1");
  m_paperSizeComboBox->addItem("B2");
  m_paperSizeComboBox->addItem("B3");
  m_paperSizeComboBox->addItem("B4");
  m_paperSizeComboBox->addItem("B5");
  m_paperSizeComboBox->addItem("B6");
  m_paperSizeComboBox->addItem("B7");
  m_paperSizeComboBox->addItem("B8");
  m_paperSizeComboBox->addItem("B9");
  m_paperSizeComboBox->addItem("B10");
  m_paperSizeComboBox->addItem("C5E");
  m_paperSizeComboBox->addItem("Comm10E");
  m_paperSizeComboBox->addItem("DLE");
  m_paperSizeComboBox->addItem("Executive");
  m_paperSizeComboBox->addItem("Folio");
  m_paperSizeComboBox->addItem("Ledger");
  m_paperSizeComboBox->addItem("Legal");
  m_paperSizeComboBox->addItem("Letter");
  m_paperSizeComboBox->addItem("Tabloid");
  m_paperSizeComboBox->addItem("Custom");
  h2a->addWidget(m_paperSizeComboBox);

  m_paperSizeEditGoupBox = new QGroupBox("", gb2);
  QHBoxLayout* h2b = new QHBoxLayout(m_paperSizeEditGoupBox);
  QLabel* widthLabel = new QLabel("width:", m_paperSizeEditGoupBox);
  m_paperWidthLineEdit = new QLineEdit(m_paperSizeEditGoupBox);
  QLabel* heightLabel = new QLabel("height:", m_paperSizeEditGoupBox);
  m_paperHeightLineEdit = new QLineEdit(m_paperSizeEditGoupBox);
  h2b->addWidget(widthLabel);
  h2b->addWidget(m_paperWidthLineEdit);
  h2b->addStretch();
  h2b->addWidget(heightLabel);
  h2b->addWidget(m_paperHeightLineEdit);

  QVBoxLayout* v2 = new QVBoxLayout(gb2);
  v2->addWidget(gb2a);
  v2->addWidget(m_paperSizeEditGoupBox);
  gb2->setLayout(v2);

  QGroupBox* gb3 = new QGroupBox("Paper Orientation", this);
  m_paperOrientationComboBox = new QComboBox(gb3);
  m_paperOrientationComboBox->addItem("Portrait");
  m_paperOrientationComboBox->addItem("Landscape");
  QHBoxLayout* h3 = new QHBoxLayout(gb3);
  h3->addWidget(m_paperOrientationComboBox);

  QGroupBox* gb4 = new QGroupBox("Resolution", this);
  QHBoxLayout* h4 = new QHBoxLayout(gb4);
  m_resolutionComboBox = new QComboBox(gb4);
  h4->addWidget(m_resolutionComboBox);

  QPrinter printer(QPrinter::HighResolution);
  if(m_fileFormatComboBox->currentText() == "PDF")
    printer.setOutputFormat(QPrinter::PdfFormat);
  else
    printer.setOutputFormat(QPrinter::PostScriptFormat);

  //QList<int> resolutions = printer.supportedResolutions();
  //QList<int>::iterator it;
  //for(it = resolutions.begin(); it != resolutions.end(); ++it)
  //{
  //  QString s;
  //  s.setNum(*it);
  //  m_resolutionComboBox->addItem(s);
  //}

  m_resolutionComboBox->addItem("72");
  m_resolutionComboBox->addItem("96");
  m_resolutionComboBox->addItem("150");
  m_resolutionComboBox->addItem("200");
  m_resolutionComboBox->addItem("300");
  m_resolutionComboBox->addItem("600");
  m_resolutionComboBox->addItem("1200");
  m_resolutionComboBox->addItem("2400");

  QGroupBox* gb5 = new QGroupBox("Page Margins (mm)", this);
  QGroupBox* gb5a = new QGroupBox("", gb5);
  QHBoxLayout* h5a = new QHBoxLayout(gb5a);
  QLabel* leftMarginLabel = new QLabel("Left:", gb5a);
  m_leftMarginSpinBox = new QSpinBox(gb5a);
  m_leftMarginSpinBox->setRange(0, 50);
  m_leftMarginSpinBox->setSingleStep(1);
  h5a->addWidget(leftMarginLabel);
  h5a->addWidget(m_leftMarginSpinBox);
  h5a->addStretch();
  QLabel* rightMarginLabel = new QLabel("Right:", gb5a);
  m_rightMarginSpinBox = new QSpinBox(gb5a);
  m_rightMarginSpinBox->setRange(0, 50);
  m_rightMarginSpinBox->setSingleStep(1);
  h5a->addWidget(rightMarginLabel);
  h5a->addWidget(m_rightMarginSpinBox);
  QVBoxLayout* v5 = new QVBoxLayout(gb5);
  v5->addWidget(gb5a);

  QGroupBox* gb5b = new QGroupBox("", gb5);
  QHBoxLayout* h5b = new QHBoxLayout(gb5b);
  QLabel* topMarginLabel = new QLabel("Top:", gb5b);
  m_topMarginSpinBox = new QSpinBox(gb5b);
  m_topMarginSpinBox->setRange(0, 50);
  m_topMarginSpinBox->setSingleStep(1);
  h5b->addWidget(topMarginLabel);
  h5b->addWidget(m_topMarginSpinBox);
  h5b->addStretch();
  QLabel* bottomMarginLabel = new QLabel("Bottom:", gb5b);
  m_bottomMarginSpinBox = new QSpinBox(gb5b);
  m_bottomMarginSpinBox->setRange(0, 50);
  m_bottomMarginSpinBox->setSingleStep(1);
  h5b->addWidget(bottomMarginLabel);
  h5b->addWidget(m_bottomMarginSpinBox);
  v5->addWidget(gb5b);
  gb5->setLayout(v5);

  QGroupBox* gb6 = new QGroupBox("File Name", this);
  QHBoxLayout* h6 = new QHBoxLayout(gb6);
  QLabel* saveLabel = new QLabel("File:", gb6);
  m_fileLineEdit = new QLineEdit(gb6);
  m_filePushButton = new QPushButton("File...", gb6);
  h6->addWidget(saveLabel);
  h6->addWidget(m_fileLineEdit);
  h6->addWidget(m_filePushButton);

  QGroupBox* gb7 = new QGroupBox(this);
  QHBoxLayout* h7 = new QHBoxLayout(gb7);
  m_okPushButton = new QPushButton("Ok", gb7);
  m_cancelPushButton = new QPushButton("Cancel", gb7);
  h7->addWidget(m_okPushButton);
  h7->addStretch();
  h7->addWidget(m_cancelPushButton);

  QVBoxLayout* v8 = new QVBoxLayout(this);
  v8->addWidget(gb1);
  v8->addWidget(gb2);
  v8->addWidget(gb3);
  v8->addWidget(gb4);
  v8->addWidget(gb5);
  v8->addWidget(gb6);
  v8->addWidget(gb7);
  setLayout(v8);

  connect(m_paperSizeComboBox, SIGNAL(activated(int)), this, SLOT(paperSizeActivatedSlot(int)));
  connect(m_fileFormatComboBox, SIGNAL(activated(int)), this, SLOT(fileFormatActivatedSlot(int)));
  connect(m_okPushButton, SIGNAL(clicked(bool)), this, SLOT(okClickedSlot(bool)));
  connect(m_cancelPushButton, SIGNAL(clicked(bool)), this, SLOT(cancelClickedSlot(bool)));
  connect(m_filePushButton, SIGNAL(clicked(bool)), this, SLOT(fileClickedSlot(bool)));

  m_paperSizeComboBox->setCurrentIndex(4);
  paperSizeActivatedSlot(m_paperSizeComboBox->currentIndex());
}

PrintInFile::~PrintInFile()
{
}

void PrintInFile::closeEvent(QCloseEvent*)
{
  cancelClickedSlot(true);
}

void PrintInFile::okClickedSlot(bool)
{
  if(m_fileLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, "Empty File Name", "Enter File Name");
    return;
  }

  accept();
  hide();
}

void PrintInFile::cancelClickedSlot(bool)
{
  reject();
  hide();
}

void PrintInFile::paperSizeActivatedSlot(int)
{
  QString s = m_paperSizeComboBox->currentText();
  if(s == "Custom")
  {
    m_paperSizeEditGoupBox->setEnabled(true);
    m_paperWidthLineEdit->setText("");
    m_paperHeightLineEdit->setText("");
  }
  else
  {
    m_paperSizeEditGoupBox->setEnabled(false);
    if(s == "A0")
    {
      m_paperWidthLineEdit->setText("841");
      m_paperHeightLineEdit->setText("1189");
    }
    else if(s == "A1")
    {
      m_paperWidthLineEdit->setText("594");
      m_paperHeightLineEdit->setText("841");
    }
    else if(s == "A2")
    {
      m_paperWidthLineEdit->setText("420");
      m_paperHeightLineEdit->setText("594");
    }
    else if(s == "A3")
    {
      m_paperWidthLineEdit->setText("297");
      m_paperHeightLineEdit->setText("420");
    }
    else if(s == "A4")
    {
      m_paperWidthLineEdit->setText("210");
      m_paperHeightLineEdit->setText("297");
    }
    else if(s == "A5")
    {
      m_paperWidthLineEdit->setText("148");
      m_paperHeightLineEdit->setText("210");
    }
    else if(s == "A6")
    {
      m_paperWidthLineEdit->setText("105");
      m_paperHeightLineEdit->setText("148");
    }
    else if(s == "A7")
    {
      m_paperWidthLineEdit->setText("74");
      m_paperHeightLineEdit->setText("105");
    }
    else if(s == "A8")
    {
      m_paperWidthLineEdit->setText("52");
      m_paperHeightLineEdit->setText("74");
    }
    else if(s == "A9")
    {
      m_paperWidthLineEdit->setText("37");
      m_paperHeightLineEdit->setText("52");
    }
    else if(s == "B0")
    {
      m_paperWidthLineEdit->setText("1000");
      m_paperHeightLineEdit->setText("1414");
    }
    else if(s == "B1")
    {
      m_paperWidthLineEdit->setText("707");
      m_paperHeightLineEdit->setText("1000");
    }
    else if(s == "B2")
    {
      m_paperWidthLineEdit->setText("500");
      m_paperHeightLineEdit->setText("707");
    }
    else if(s == "B3")
    {
      m_paperWidthLineEdit->setText("353");
      m_paperHeightLineEdit->setText("500");
    }
    else if(s == "B4")
    {
      m_paperWidthLineEdit->setText("250");
      m_paperHeightLineEdit->setText("353");
    }
    else if(s == "B5")
    {
      m_paperWidthLineEdit->setText("176");
      m_paperHeightLineEdit->setText("250");
    }
    else if(s == "B6")
    {
      m_paperWidthLineEdit->setText("125");
      m_paperHeightLineEdit->setText("176");
    }
    else if(s == "B7")
    {
      m_paperWidthLineEdit->setText("88");
      m_paperHeightLineEdit->setText("125");
    }
    else if(s == "B8")
    {
      m_paperWidthLineEdit->setText("62");
      m_paperHeightLineEdit->setText("88");
    }
    else if(s == "B9")
    {
      m_paperWidthLineEdit->setText("33");
      m_paperHeightLineEdit->setText("62");
    }
    else if(s == "B10")
    {
      m_paperWidthLineEdit->setText("31");
      m_paperHeightLineEdit->setText("44");
    }
    else if(s == "C5E")
    {
      m_paperWidthLineEdit->setText("163");
      m_paperHeightLineEdit->setText("229");
    }
    else if(s == "Comm10E")
    {
      m_paperWidthLineEdit->setText("105");
      m_paperHeightLineEdit->setText("241");
    }
    else if(s == "DLE")
    {
      m_paperWidthLineEdit->setText("110");
      m_paperHeightLineEdit->setText("220");
    }
    else if(s == "Executive")
    {
      m_paperWidthLineEdit->setText("190.5");
      m_paperHeightLineEdit->setText("254");
    }
    else if(s == "Folio")
    {
      m_paperWidthLineEdit->setText("210");
      m_paperHeightLineEdit->setText("330");
    }
    else if(s == "Ledger")
    {
      m_paperWidthLineEdit->setText("431.8");
      m_paperHeightLineEdit->setText("279.4");
    }
    else if(s == "Legal")
    {
      m_paperWidthLineEdit->setText("215.9");
      m_paperHeightLineEdit->setText("355.6");
    }
    else if(s == "Letter")
    {
      m_paperWidthLineEdit->setText("215.9");
      m_paperHeightLineEdit->setText("279.4");
    }
    else if(s == "Tabloid")
    {
      m_paperWidthLineEdit->setText("279.4");
      m_paperHeightLineEdit->setText("431.8");
    }
  }
}

void PrintInFile::fileFormatActivatedSlot(int)
{
  QString file = m_fileLineEdit->text();

  if(file.isEmpty())
    return;

  setFileName(file);
}

void PrintInFile::fileClickedSlot(bool)
{
  QString file = QFileDialog::getSaveFileName(this, "Save File");

  if(file.isEmpty())
    return;

  setFileName(file);
}

void PrintInFile::setFileName(QString file)
{
  if(m_fileFormatComboBox->currentText() == "PDF")
  {
    int p = file.toUpper().indexOf(".PS");
    if(p != -1)
      file.replace(p, 3, ".pdf");
    else
    {
      if(file.toUpper().contains(".PDF") == false)
        file.append(".pdf");
    }
  }
  else
  {
    int p = file.toUpper().indexOf(".PDF");
    if(p != -1)
      file.replace(p, 4, ".ps");
    else
    {
      if(file.toUpper().contains(".PS") == false)
      file.append(".ps");
    }
  }

  m_fileLineEdit->setText(file);
}