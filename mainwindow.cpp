#include "mainwindow.h"

#include <iostream>

#include "ui_mainwindow.h"

void MainWindow::replyFinished(QNetworkReply *reply) {
  if (reply->error() != QNetworkReply::NoError) {
    //处理中的错误信息
    qDebug() << "reply error:" << reply->errorString();
  } else {
    //请求方式
    qDebug() << "operation:" << reply->operation();
    //状态码
    qDebug()
        << "status code:"
        << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "url:" << reply->url();
    // qDebug()<<"raw header:"<<reply->rawHeaderList();

    //获取响应信息
    const QByteArray reply_data = reply->readAll();
    QString str(reply_data);
    qDebug() << "read all:" << str;
    m_oneWords->setText(str);
  }
  reply->deleteLater();
}

void MainWindow::ontimeOut() { GetOneWords(); }

void MainWindow::GetOneWords() {
  QNetworkRequest request;

  QString urlString;
  QUrl url;
  urlString.append("https://v1.hitokoto.cn/?c=d&c=h&encode=text");
  url.setUrl(urlString);

  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  conf.setProtocol(QSsl::AnyProtocol);
  request.setSslConfiguration(conf);

  request.setUrl(url);

  m_pNetAccMgr->get(request);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->tabWidget->tabsClosable();

  m_pNetAccMgr = new QNetworkAccessManager(this);
  connect(m_pNetAccMgr, &QNetworkAccessManager::finished, this,
          &MainWindow::replyFinished);
  connect(&m_timer, SIGNAL(timeout()), this, SLOT(ontimeOut()));
  connect(ui->fileImp, SIGNAL(triggered()), this, SLOT(fileImp_triggered()));

  m_oneWords = new QLabel("想得到的都拥有，得不到的都释怀。", this);
  // statusBar()->addWidget(m_oneWords); //靠左
  statusBar()->addPermanentWidget(m_oneWords);  //靠右
  m_timer.start(10000);

  formTable = nullptr;
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::fileImp_triggered() {
  QString aFile = QFileDialog::getOpenFileName(this, "选择数据库文件", "",
                                               "SQLite数据库(*.db *.db3)");
  if (aFile.isEmpty()) {

    return;
    /*
    QMessageBox::warning(this, "提示", "未选择数据库，\n将使用默认数据库",
                         QMessageBox::Ok, QMessageBox::NoButton);

    formTable = new QFormTable(this);
    */
  } else {
    formTable = new QFormTable(this, aFile);
  }

  formTable->setAttribute(Qt::WA_DeleteOnClose);
  int cur = ui->tabWidget->addTab(
      formTable, QString::asprintf("FAQ %d", ui->tabWidget->count()));

  ui->tabWidget->setCurrentIndex(cur);
  ui->tabWidget->setVisible(true);
  ui->editModity->setEnabled(true);

  ui->fileImp->setEnabled(false);
}

void MainWindow::on_editModity_triggered() {
  if (formTable != nullptr) {
    formTable->tableView_readwrite();
    ui->editModity->setEnabled(false);
    ui->editReadOnly->setEnabled(true);
    ui->editAdd->setEnabled(true);
  }
}

void MainWindow::on_editReadOnly_triggered() {
  if (formTable != nullptr) {
    bool ret = formTable->tableView_save();
    if (ret) {
      formTable->tableView_readonly();
      ui->editReadOnly->setEnabled(false);
      ui->editModity->setEnabled(true);
      ui->editAdd->setEnabled(false);
    }
  }
}

void MainWindow::on_editAdd_triggered() {
  formTable->tableView_append();
  ui->editSave->setEnabled(true);
  ui->editCancel->setEnabled(true);
}

void MainWindow::on_editSave_triggered() {
  bool ret = formTable->tableView_save();
  if (ret) {
    ui->editSave->setEnabled(false);
    ui->editCancel->setEnabled(false);
  }
}

void MainWindow::on_editCancel_triggered() {
  formTable->tableView_cancel();
  ui->editSave->setEnabled(false);
  ui->editCancel->setEnabled(false);
}

void MainWindow::on_actionClose_triggered() {
  if (formTable != nullptr) delete formTable;
}
