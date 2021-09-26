#include "qformtable.h"

#include "ui_qformtable.h"

bool QFormTable::openDatabase(const QString &aFile) {
  DB = QSqlDatabase::addDatabase("QSQLITE");
  DB.setDatabaseName(aFile);
  if (!DB.open()) {
    QMessageBox::warning(this, "错误", "打开数据库失败", QMessageBox::Ok,
                         QMessageBox::NoButton);
    return false;
  }

  return true;
}

void QFormTable::openTable() {
  tabModel = new QSqlTableModel(this, DB);
  tabModel->setTable("FAQ");
  tabModel->setSort(tabModel->fieldIndex("Id"), Qt::AscendingOrder);
  tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

  if (!(tabModel->select())) {
    QMessageBox::critical(
        this, "错误信息",
        "打开数据表错误,错误信息\n" + (tabModel->lastError().text()),
        QMessageBox::Ok, QMessageBox::NoButton);
    return;
  }

  tabModel->setHeaderData(tabModel->fieldIndex("FAQId"), Qt::Horizontal,
                          "编号");
  tabModel->setHeaderData(tabModel->fieldIndex("FAQName"), Qt::Horizontal,
                          "题目");
  tabModel->setHeaderData(tabModel->fieldIndex("QuestionTypeId"),
                          Qt::Horizontal, "类别");
  tabModel->setHeaderData(tabModel->fieldIndex("CreateTime"), Qt::Horizontal,
                          "创建时间");
  tabModel->setHeaderData(tabModel->fieldIndex("Answer"), Qt::Horizontal,
                          "答案");

  theSelection = new QItemSelectionModel(tabModel);

  ui->tableView->setModel(tabModel);
  ui->tableView->setSelectionModel(theSelection);
  ui->tableView->setColumnHidden(tabModel->fieldIndex("QuestionTypeId"), true);
  ui->tableView->setColumnHidden(tabModel->fieldIndex("CreateTime"), true);
  ui->tableView->setColumnHidden(tabModel->fieldIndex("Answer"), true);

  ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
  ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->tableView->setAlternatingRowColors(true);
  ui->tableView->resizeColumnsToContents();

  ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  get_fieldnames();
  isEditable = false;
  ui->cbSort->setCurrentIndex(tabModel->fieldIndex("FAQId"));
  ui->rbInc->setChecked(true);
}

QFormTable::QFormTable(QWidget *parent, const QString &dbfile)
    : QMainWindow(parent), ui(new Ui::QFormTable) {
  ui->setupUi(this);
  ui->preview->setContextMenuPolicy(Qt::NoContextMenu);

  connect(ui->rbAll, SIGNAL(clicked()), this, SLOT(rbAll_clicked()));
  connect(ui->rbBasic, SIGNAL(clicked()), this, SLOT(rbBasic_clicked()));
  connect(ui->rbAdvance, SIGNAL(clicked()), this, SLOT(rbAdvance_clicked()));
  connect(ui->rbEffective, SIGNAL(clicked()), this,
          SLOT(rbEffective_clicked()));
  connect(ui->rbSystem, SIGNAL(clicked()), this, SLOT(rbSystem_clicked()));
  connect(ui->rbNetwork, SIGNAL(clicked()), this, SLOT(rbNetwork_clicked()));
  connect(ui->rbDataStru, SIGNAL(clicked()), this, SLOT(rbDataStru_clicked()));
  connect(ui->rbAlgorithm, SIGNAL(clicked()), this,
          SLOT(rbAlgorithm_clicked()));
  connect(ui->rbDatabase, SIGNAL(clicked()), this, SLOT(rbDatabase_clicked()));
  connect(ui->rbDesignmode, SIGNAL(clicked()), this,
          SLOT(rbDesignmode_clicked()));
  connect(ui->rbOS, SIGNAL(clicked()), this, SLOT(rbOS_clicked()));

  if (false == openDatabase(dbfile)) return;

  openTable();

  //设置web channel
  ui->preview->setContextMenuPolicy(Qt::NoContextMenu);
  page = new PreviewPage(this);
  ui->preview->setPage(page);
  channel = new QWebChannel(this);
  m_content.setText("# MAKEDOWN or WebPage");

  channel->registerObject(QStringLiteral("content"), &m_content);
  page->setWebChannel(channel);

  ui->preview->setUrl(QUrl("qrc:/index.html"));
}

QFormTable::~QFormTable() {
  tableView_save();
  delete theSelection;
  delete tabModel;
  delete ui;
}

void QFormTable::get_fieldnames() {
  QSqlRecord emptyRec = tabModel->record();
  for (int i = 0; i < emptyRec.count() - 1; ++i)
    ui->cbSort->addItem(tabModel->headerData(i, Qt::Horizontal).toString());
  // ui->cbSort->addItem(emptyRec.fieldName(i));
}

void QFormTable::rbAll_clicked() { tabModel->setFilter(""); }

void QFormTable::rbBasic_clicked() { tabModel->setFilter("QuestionTypeId=1"); }

void QFormTable::rbAdvance_clicked() {
  tabModel->setFilter("QuestionTypeId=2");
}

void QFormTable::rbEffective_clicked() {
  tabModel->setFilter("QuestionTypeId=3");
}

void QFormTable::rbSystem_clicked() { tabModel->setFilter("QuestionTypeId=4"); }

void QFormTable::rbNetwork_clicked() {
  tabModel->setFilter("QuestionTypeId=5");
}

void QFormTable::rbDataStru_clicked() {
  tabModel->setFilter("QuestionTypeId=6");
}

void QFormTable::rbAlgorithm_clicked() {
  tabModel->setFilter("QuestionTypeId=7");
}

void QFormTable::rbDatabase_clicked() {
  tabModel->setFilter("QuestionTypeId=8");
}

void QFormTable::rbDesignmode_clicked() {
  tabModel->setFilter("QuestionTypeId=9");
}

void QFormTable::rbOS_clicked() { tabModel->setFilter("QuestionTypeId=10"); }

void QFormTable::on_tableView_clicked(const QModelIndex &index) {
  int curRecNo = index.row();
  QSqlRecord curRec = tabModel->record(curRecNo);
  if (curRec.isNull("Answer") && isEditable == false) {
    QMessageBox::information(this, "信息", "尚未为此问题放置答案",
                             QMessageBox::Ok, QMessageBox::NoButton);
  } else {
    if (curRec.value("Answer").toString().startsWith("http")) {
      ui->preview->setPage(nullptr);
      ui->preview->setUrl(QUrl(curRec.value("Answer").toString()));
    } else {
      qDebug() << "0";
      ui->preview->setPage(page);
      m_content.setText(curRec.value("Answer").toString());
    }
  }
}

void QFormTable::tableView_readonly() {
  ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableView->setColumnHidden(tabModel->fieldIndex("QuestionTypeId"), true);
  ui->tableView->setColumnHidden(tabModel->fieldIndex("CreateTime"), true);
  ui->tableView->setColumnHidden(tabModel->fieldIndex("Answer"), true);
  isEditable = false;
}

void QFormTable::tableView_readwrite() {
  ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
  ui->tableView->setColumnHidden(tabModel->fieldIndex("QuestionTypeId"), false);
  ui->tableView->setColumnHidden(tabModel->fieldIndex("CreateTime"), false);
  ui->tableView->setColumnHidden(tabModel->fieldIndex("Answer"), false);
  isEditable = true;
}

void QFormTable::tableView_append() {
  tabModel->insertRow(tabModel->rowCount(), QModelIndex());
  QModelIndex curIndex = tabModel->index(tabModel->rowCount() - 1, 1);
  theSelection->clearSelection();
  theSelection->setCurrentIndex(curIndex, QItemSelectionModel::Select);
  int currow = curIndex.row();
  tabModel->setData(tabModel->index(currow, 0), currow + 1);
  QDateTime dtime(QDateTime::currentDateTime());
  tabModel->setData(tabModel->index(currow, 3),
                    dtime.toString("yyyy-MM-dd hh:mm:ss.zzz"));
}

bool QFormTable::tableView_save() {
  bool res = tabModel->submitAll();
  if (!res) {
    QMessageBox::information(this, "消息",
                             "数据保存错误:\n" + tabModel->lastError().text(),
                             QMessageBox::Ok, QMessageBox::NoButton);
  }

  return res;
}

void QFormTable::tableView_cancel() { tabModel->revertAll(); }

void QFormTable::on_rbInc_clicked() {
  tabModel->setSort(ui->cbSort->currentIndex(), Qt::AscendingOrder);
  tabModel->select();
}

void QFormTable::on_rbDec_clicked() {
  tabModel->setSort(ui->cbSort->currentIndex(), Qt::DescendingOrder);
  tabModel->select();
}

void QFormTable::on_cbSort_currentIndexChanged(int index) {
  if (ui->rbDec->isChecked())
    tabModel->setSort(index, Qt::DescendingOrder);
  else
    tabModel->setSort(index, Qt::AscendingOrder);
  tabModel->select();
}
