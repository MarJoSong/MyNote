#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDataWidgetMapper>
#include <QFileDialog>
#include <QItemSelectionModel>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include "qformtable.h"
#include "stdafx.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 private:
 private:
  QSqlDatabase DB;
  QSqlTableModel *tabModel;
  QItemSelectionModel *theSelection;
  QDataWidgetMapper *dataMappler;
  void openTable();
  void getFieldNames();

 private slots:
  void replyFinished(QNetworkReply *reply);
  void ontimeOut();

  void fileImp_triggered();

  void on_editModity_triggered();

  void on_editReadOnly_triggered();

  void on_editAdd_triggered();

  void on_editSave_triggered();

  void on_editCancel_triggered();

  void on_actionClose_triggered();

  void on_editDel_triggered();

protected:
  void GetOneWords();

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
  QNetworkAccessManager *m_pNetAccMgr;
  QLabel *m_oneWords;
  QTimer m_timer;
  QFormTable *formTable;
};
#endif  // MAINWINDOW_H
