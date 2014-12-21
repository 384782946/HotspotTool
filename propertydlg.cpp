#include "propertydlg.h"
#include "ui_propertydlg.h"
#include <QResizeEvent>
#include <QComboBox>

#include <QState>
#include <QStateMachine>
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <QSignalTransition>
#include <QVBoxLayout>
#include <QPushButton>

PropertyDlg::PropertyDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PropertyDlg)
{
    m_CurrentItem = NULL;
    m_TypeNameList<<tr("Rect")<<tr("Circle")<<tr("Polygon");

    ui->setupUi(this);
    setLayout(ui->verticalLayout);
}

PropertyDlg::~PropertyDlg()
{
    delete ui;
}

QSize PropertyDlg::sizeHint() const
{
    return QSize(300, 900);
}

void PropertyDlg::InitProperty(QGraphicsItem *item,QGraphicsItem *parent)
{
    m_CurrentItem = item;
    ui->tableWidget->reset();
    if(!item) return;

    if(parent)
    {
        int type = 0;
        int nVisible = 0;
        QString strKey;
        float px,py,pw,ph;

        nVisible = item->data(FLAGS_VISIBLE).toInt();
        strKey = item->data(FLAGS_KEY).toString();
        type = item->data(FLAGS_TYPE).toInt();
        px = item->data(FLAGS_X).toDouble();
        py = item->data(FLAGS_Y).toFloat();
        pw = item->data(FLAGS_W).toFloat();
        ph = item->data(FLAGS_H).toFloat();

        ui->tableWidget->setRowCount(FLAGS_END);

        ui->tableWidget->setItem(FLAGS_VISIBLE,0,new QTableWidgetItem(tr("Visible")));
        QComboBox *comBox = new QComboBox();
        comBox->addItem("FALSE");
        comBox->addItem("TRUE");
        comBox->setCurrentIndex(nVisible);
        connect(comBox,SIGNAL(currentIndexChanged(int)),this,SLOT(on_visible_change(int)));
        ui->tableWidget->setCellWidget(FLAGS_VISIBLE,1,comBox);

        ui->tableWidget->setItem(FLAGS_KEY,0,new QTableWidgetItem(tr("Key")));
        ui->tableWidget->setItem(FLAGS_KEY,1,new QTableWidgetItem(tr("%1").arg(strKey)));

        ui->tableWidget->setItem(FLAGS_TYPE,0,new QTableWidgetItem(tr("Type")));
        ui->tableWidget->setItem(FLAGS_TYPE,1,new QTableWidgetItem(tr("%1").arg(m_TypeNameList[type])));

        ui->tableWidget->setItem(FLAGS_X,0,new QTableWidgetItem(tr("X")));
        ui->tableWidget->setItem(FLAGS_X,1,new QTableWidgetItem(tr("%1").arg(px)));

        ui->tableWidget->setItem(FLAGS_Y,0,new QTableWidgetItem(tr("Y")));
        ui->tableWidget->setItem(FLAGS_Y,1,new QTableWidgetItem(tr("%1").arg(py)));

        ui->tableWidget->setItem(FLAGS_W,0,new QTableWidgetItem(tr("Width")));
        ui->tableWidget->setItem(FLAGS_W,1,new QTableWidgetItem(tr("%1").arg(pw)));

        ui->tableWidget->setItem(FLAGS_H,0,new QTableWidgetItem(tr("Height")));
        ui->tableWidget->setItem(FLAGS_H,1,new QTableWidgetItem(tr("%1").arg(ph)));
    }
    else
    {
        QRectF itemRect = item->sceneBoundingRect();
        int index = 0;
        float px,py,pw,ph;
        px = itemRect.x();
        py = itemRect.y();
        pw = itemRect.width();
        ph = itemRect.height();

        ui->tableWidget->setRowCount(4);

        ui->tableWidget->setItem(index,0,new QTableWidgetItem(tr("X")));
        ui->tableWidget->setItem(index++,1,new QTableWidgetItem(tr("%1").arg(px)));

        ui->tableWidget->setItem(index,0,new QTableWidgetItem(tr("Y")));
        ui->tableWidget->setItem(index++,1,new QTableWidgetItem(tr("%1").arg(py)));

        ui->tableWidget->setItem(index,0,new QTableWidgetItem(tr("Width")));
        ui->tableWidget->setItem(index++,1,new QTableWidgetItem(tr("%1").arg(pw)));

        ui->tableWidget->setItem(index,0,new QTableWidgetItem(tr("Height")));
        ui->tableWidget->setItem(index++,1,new QTableWidgetItem(tr("%1").arg(ph)));
    }

    for(int i=0;i<ui->tableWidget->rowCount();++i)
    {
        QTableWidgetItem *item = ui->tableWidget->item(i,0);
        if(item)
        {
            item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        }
    }
}

void PropertyDlg::on_tableWidget_cellChanged(int row, int column)
{
     QTableWidgetItem *tableWidget = ui->tableWidget->item(row,column);

     if(tableWidget->isSelected() &&
             ui->tableWidget->item(row,0)->text() == "Key")
     {
        emit itemChanged(row,tableWidget->text());
     }
}

void PropertyDlg::on_visible_change(int index)
{
    if(m_CurrentItem)
    {
        m_CurrentItem->setData(FLAGS_VISIBLE,index);
    }
}
