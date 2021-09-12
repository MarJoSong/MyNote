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
  QFormTable(QWidget *parent = nullptr, const QString &dbfile = "D:\\faq.db");
  ~QFormTable();

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
  void rbSystem_clicked();
  void rbNetwork_clicked();
  void rbDataStru_clicked();
  void rbAlgorithm_clicked();
  void rbDatabase_clicked();
  void rbDesignmode_clicked();
  void rbOS_clicked();
  void on_tableView_clicked(const QModelIndex &index);
  void on_tableView_doubleClicked(const QModelIndex &index);

  void on_rbEffective_clicked();

 private:
  Ui::QFormTable *ui;

  QSqlDatabase DB;
  QSqlTableModel *tabModel;
  QDataWidgetMapper *dataMappler;
  QStandardItemModel *theModel;       //数据模型
  QItemSelectionModel *theSelection;  // Item选择模型
  Document m_content;
  bool isEditable;
};

#endif  // QFORMTABLE_H
