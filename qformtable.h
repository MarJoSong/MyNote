#ifndef QFORMTABLE_H
#define QFORMTABLE_H

#include <QDataWidgetMapper>
#include <QFileDialog>
#include <QFontDatabase>
#include <QItemSelectionModel>
#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QWebChannel>

#include "document.h"
#include "previewpage.h"
#include "stdafx.h"

namespace Ui {
class QFormTable;
}

class QFormTable : public QMainWindow {
  Q_OBJECT

 private:
  bool openDatabase(const QString &aFile);
  void openTable();

 public:
  QFormTable(QWidget *parent = nullptr,
             const QString &dbfile = "D:\\git\\Road2Offer\\faq.db");
  ~QFormTable();

  void get_fieldnames();
  void tableView_readonly();
  void tableView_readwrite();
  void tableView_append();
  bool tableView_save();
  void tableView_cancel();

 signals:

 private slots:

  void rbAll_clicked();
  void rbBasic_clicked();
  void rbAdvance_clicked();
  void rbEffective_clicked();
  void rbSystem_clicked();
  void rbNetwork_clicked();
  void rbDataStru_clicked();
  void rbAlgorithm_clicked();
  void rbDatabase_clicked();
  void rbDesignmode_clicked();
  void rbOS_clicked();
  void on_tableView_clicked(const QModelIndex &index);

  void on_rbInc_clicked();

  void on_rbDec_clicked();

  void on_cbSort_currentIndexChanged(int index);

 private:
  Ui::QFormTable *ui;

  QSqlDatabase DB;
  QSqlTableModel *tabModel;
  QDataWidgetMapper *dataMappler;
  QItemSelectionModel *theSelection;  // Item选择模型
  PreviewPage *page;
  QWebChannel *channel;
  Document m_content;
  bool isEditable;
};

#endif  // QFORMTABLE_H
