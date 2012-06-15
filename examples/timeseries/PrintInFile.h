#ifndef __PRINT_IN_FILE_H_
#define __PRINT_IN_FILE_H_
// QT
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QFileDialog>
#include <QCloseEvent>

// TerraLib
class PrintInFile : public QDialog
{
  Q_OBJECT

public:
  PrintInFile(QWidget* parent = 0);
  ~PrintInFile();

  void closeEvent(QCloseEvent*);

protected slots:
  void okClickedSlot(bool);
  void cancelClickedSlot(bool);
  void paperSizeActivatedSlot(int);
  void fileFormatActivatedSlot(int);
  void fileClickedSlot(bool);

private:
  void setFileName(QString);

public:
  QComboBox* m_fileFormatComboBox;
  QLineEdit* m_fileLineEdit;
  QPushButton* m_filePushButton;
  QComboBox* m_paperSizeComboBox;
  QGroupBox* m_paperSizeEditGoupBox;
  QLineEdit* m_paperWidthLineEdit;
  QLineEdit* m_paperHeightLineEdit;
  QComboBox* m_paperOrientationComboBox;
  QComboBox* m_resolutionComboBox;
  QSpinBox* m_leftMarginSpinBox;
  QSpinBox* m_rightMarginSpinBox;
  QSpinBox* m_topMarginSpinBox;
  QSpinBox* m_bottomMarginSpinBox;
  QPushButton* m_okPushButton;
  QPushButton* m_cancelPushButton;
};
#endif
