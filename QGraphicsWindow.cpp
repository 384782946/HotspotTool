#include "QGraphicsWindow.h"
#include "ui_QGraphicsWindow.h"
#include <QFileDialog>
#include <QToolBar>
#include "propertydlg.h"
#include <QMessageBox>
#include <QPushButton>

QGraphicsWindow::QGraphicsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QGraphicsWindow)
{
    m_MainView = NULL;
    m_MainScene = NULL;
    m_MainImage = NULL;
    m_pDrawToolbar = NULL;
    m_pToolbar = NULL;
    m_pPropertyDlg = NULL;
    m_SelectItem = NULL;
    m_bIsModify = false;

    ui->setupUi(this);
    showMaximized();
    InitToolbar();

    m_MainView = new QMyView(this);
    m_MainScene = new QGraphicsScene(m_MainView);
    m_MainView->setScene(m_MainScene);
    setCentralWidget(m_MainView);

    m_pPropertyDlg = new QDockWidget("Property Dialog",this);
    PropertyDlg *pPropertyDlg = new PropertyDlg;
    m_pPropertyDlg->setWidget(pPropertyDlg);
    m_pPropertyDlg->setFocusPolicy(Qt::StrongFocus);
    addDockWidget(Qt::RightDockWidgetArea,m_pPropertyDlg);

    connect(pPropertyDlg,SIGNAL(itemChanged(int,QString)),this,SLOT(on_item_changed(int,QString)));
    m_MainView->setFocus();
}

QGraphicsWindow::~QGraphicsWindow()
{
    delete ui;
}

void QGraphicsWindow::InitToolbar()
{
    m_pToolbar = addToolBar(tr("System Toolbar"));
    if(m_pToolbar)
    {
        m_pToolbar->setAllowedAreas(Qt::TopToolBarArea);
        m_pToolbar->addAction(ui->action_open);
        m_pToolbar->addAction(ui->action_load);
        m_pToolbar->addAction(ui->action_save);
        m_pToolbar->addAction(ui->action_close);
        m_pToolbar->addSeparator();
        m_pToolbar->addAction(ui->action_zoomin);
        m_pToolbar->addAction(ui->action_zoomout);

		m_pToolbar->addSeparator();

		m_ColorBtn = new QPushButton(this);
		m_ColorBtn->setFixedSize(m_pToolbar->iconSize());
		connect(m_ColorBtn,SIGNAL(clicked()),this,SLOT(on_color_puton_triggered()));
		m_ColorBtn->setStyleSheet(QString("background-color:%1").arg(QColor(Qt::black).name()));
		m_pToolbar->addWidget(m_ColorBtn);

        ui->action_view1->setChecked(true);
    }

    m_pDrawToolbar  = new QToolBar(tr("Draw ToolBar"));
    if(m_pDrawToolbar)
    {
        m_pDrawToolbar->setAllowedAreas(Qt::AllToolBarAreas);
        m_pDrawToolbar->addAction(ui->action_select);
        m_pDrawToolbar->addAction(ui->action_move);
        m_pDrawToolbar->addAction(ui->action_scale1);
        m_pDrawToolbar->addSeparator();
        m_pDrawToolbar->addAction(ui->action_rect);
        m_pDrawToolbar->addAction(ui->action_circle);
        m_pDrawToolbar->addAction(ui->action_polygon);

        QActionGroup* ag = new QActionGroup(this);
        ag->addAction(ui->action_select);
        ag->addAction(ui->action_move);
        ag->addAction(ui->action_scale1);
        ag->addAction(ui->action_rect);
        ag->addAction(ui->action_circle);
        ag->addAction(ui->action_polygon);

        addToolBar(Qt::LeftToolBarArea,m_pDrawToolbar);

        ui->action_select->setChecked(true);
        ui->action_view2->setChecked(true);
    }
}

void QGraphicsWindow::on_action_open_triggered()
{
   on_action_close_triggered();

   QString path = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("Images (*.png *.bmp *.jpg)"));
   if(path.isEmpty()) return;

   m_ImageName = path;
   QPixmap pm;
   pm.load(m_ImageName);
   if(m_MainImage)
   {
        QGraphicsPixmapItem* gpi= (QGraphicsPixmapItem*)m_MainImage;
        gpi->setPixmap(pm);
   }
   else
   {
        m_MainImage =  m_MainScene->addPixmap(pm);
   }

   m_MainImage->setPos(0,0);
   m_MainImage->show();
   m_MainScene->setSceneRect(0,0,pm.width(),pm.height());
   if(m_MainView->isTransformed())
       m_MainView->resetTransform();
}

void QGraphicsWindow::on_action_save_triggered()
{
    if(m_XmlFileName.isEmpty())
    {
        on_action_saveAs_triggered();
    }
    else
    {
        SaveXml(m_XmlFileName);
        m_bIsModify = false;
    }
}

void QGraphicsWindow::on_action_saveAs_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, tr("save file"), " ",  tr("XML (*.xml)"));
    if(!path.isEmpty())
    {
        m_XmlFileName = path;
        on_action_save_triggered();
    }
}

void QGraphicsWindow::on_action_rect_triggered()
{
    if(ui->action_rect->isChecked())
    {
        m_MainView->m_Mode = QMyView::DRAW_MODE_RECT;
		m_MainView->setFocus();
    }
    else
    {
        m_MainView->m_Mode = QMyView::DRAW_MODE_SELECT;
    }
}

void QGraphicsWindow::on_action_circle_triggered()
{
    if(ui->action_circle->isChecked())
    {
        m_MainView->m_Mode = QMyView::DRAW_MODE_CIRCLE;
		m_MainView->setFocus();
    }
    else
    {
        m_MainView->m_Mode = QMyView::DRAW_MODE_SELECT;
        ui->action_circle->setChecked(false);
    }
}

void QGraphicsWindow::on_action_move_triggered()
{
    if(ui->action_move->isChecked())
    {
        m_MainView->m_Mode = QMyView::DRAW_MODE_MOVE;
		m_MainView->setFocus();
    }
    else
    {
        m_MainView->m_Mode = QMyView::DRAW_MODE_SELECT;
        ui->action_move->setChecked(false);
    }
}

void QGraphicsWindow::on_action_select_triggered()
{
    if(ui->action_select->isChecked())
    {
        m_MainView->m_Mode = QMyView::DRAW_MODE_SELECT;
		m_MainView->setFocus();
    }
    else
    {
        m_MainView->m_Mode = QMyView::DRAW_MODE_SELECT;
        ui->action_select->setChecked(false);
    }
}

void QGraphicsWindow::on_action_scale1_triggered()
{
    if(ui->action_scale1->isChecked())
    {
        m_MainView->m_Mode = QMyView::DRAW_MODE_SCALE;
		m_MainView->setFocus();
    }
    else
    {
        m_MainView->m_Mode = QMyView::DRAW_MODE_SELECT;
        ui->action_scale1->setChecked(false);
    }
}

void QGraphicsWindow::on_action_polygon_triggered()
{
    if(ui->action_polygon->isChecked())
    {
          m_MainView->m_Mode = QMyView::DRAW_MODE_POLYGON;
		  m_MainView->setFocus();
     }
     else
     {
          m_MainView->m_Mode = QMyView::DRAW_MODE_SELECT;
          ui->action_polygon->setChecked(false);
     }
}

void QGraphicsWindow::on_action_close_triggered()
{
    if(m_MainImage)
    {
        if(m_bIsModify)
        {
            if(QMessageBox::Yes == QMessageBox::information(this, tr("Warning"), tr("Do you want to save?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
            {
                on_action_save_triggered();
            }
        }

        m_MainScene->clear();
        m_MainImage = NULL;
        m_MainView->m_FocusItem = NULL;
        m_MainView->m_LastFocusItem = NULL;
        m_MainView->m_CopyItem = NULL;
        m_SelectItem = NULL;
    }
}

void QGraphicsWindow::on_action_load_triggered()
{
     QString path = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("XML (*.xml)"));
     if(!path.isEmpty())
         OpenXml(path);
}

#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>
#include <QGraphicsItem>

bool QGraphicsWindow::OpenXml(QString fileName)
{
    if(m_MainImage == NULL) return false;

	if(QMessageBox::No == QMessageBox::warning(this,"Warning","Do you want reserve current hotspot?",QMessageBox::Yes|QMessageBox::No))
	{
		QList<QGraphicsItem*> itemList = m_MainScene->items();
		foreach(QGraphicsItem *item,itemList)
		{
			if (item != m_MainImage)
			{
				m_MainScene->removeItem(item);
			}
		}
	}

    QDomDocument doc; //(fileName);
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    if (!doc.setContent(&file)) {
        file.close();
        return false;
    }
    file.close();

    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = doc.documentElement();

	QColor myColor(docElem.attribute("color"));
	if (myColor.isValid())
	{
		m_ColorBtn->setStyleSheet(QString("background-color:%1").arg(myColor.name()));
	}
	else
	{
		myColor = QColor(Qt::black);
	}

	m_MainView->changePenColor(myColor);

    QDomElement e = docElem.firstChildElement();
    while(!e.isNull())
    {
        QString strVisible = e.attribute("VISIBLE");
        QString strKey = e.attribute(tr("KEY"));
        QString strType = e.attribute(tr("TYPE"));
        QString strX = e.attribute(tr("CX"));
        QString strY = e.attribute(tr("CY"));
        QString strW = e.attribute(tr("CW"));
        QString strH = e.attribute(tr("CH"));

        if(strVisible.isEmpty() || strKey.isEmpty() || strType.isEmpty() || strX.isEmpty()
                || strY.isEmpty() || strW.isEmpty() || strH.isEmpty())
        {
            //qDebug()<<"xml文件可能有错误";
            QMessageBox::warning(this,tr("Warning"),tr("xml error"));
            return false;
        }
        else
        {
            int nVisible = strVisible.toInt();
            int nType = strType.toInt();
            float fCx = strX.toFloat();
            float fCy = strY.toFloat();
            float fCw = strW.toFloat();
            float fCh = strH.toFloat();
            QRectF rect,parentRect;
            parentRect = m_MainImage->sceneBoundingRect();

            rect.setX(parentRect.x() + parentRect.width()*fCx);
            rect.setY(parentRect.y() + parentRect.height()*fCy);
            rect.setWidth(parentRect.width()*fCw);
            rect.setHeight(parentRect.height()*fCh);

            if(nType == QMyView::ITEM_RECT)
            {
                QGraphicsRectItem *pRect = m_MainScene->addRect(rect,QPen(myColor));
                if(pRect)
                {
                    pRect->setData(FLAGS_VISIBLE,nVisible);
                    pRect->setData(FLAGS_KEY,strKey); // 标示符
                    pRect->setData(FLAGS_TYPE,nType); // 类型：矩形还是圆
                    pRect->setData(FLAGS_X,fCx); //x
                    pRect->setData(FLAGS_Y,fCy); //y
                    pRect->setData(FLAGS_W,fCw); //w
                    pRect->setData(FLAGS_H,fCh); //h
                }
            }
            else if(nType == QMyView::ITEM_CIRCLE)
            {
                QGraphicsEllipseItem *pEllipse = m_MainScene->addEllipse(rect,QPen(myColor));
                if(pEllipse)
                {
                    pEllipse->setData(FLAGS_VISIBLE,nVisible);
                    pEllipse->setData(FLAGS_KEY,strKey); // 标示符
                    pEllipse->setData(FLAGS_TYPE,nType); // 类型：矩形还是圆
                    pEllipse->setData(FLAGS_X,fCx); //x
                    pEllipse->setData(FLAGS_Y,fCy); //y
                    pEllipse->setData(FLAGS_W,fCw); //w
                    pEllipse->setData(FLAGS_H,fCh); //h
                }
            }
            else if(nType == QMyView::ITEM_POLYGON)
            {
                QDomElement child = e.firstChildElement();
                QPolygonF polygon;
                while (!child.isNull()) {
                    float cx = child.attribute(tr("CX")).toFloat();
                    float cy = child.attribute(tr("CY")).toFloat();

                    polygon<<QPointF(cx*parentRect.width(),cy*parentRect.height());
                    child = child.nextSiblingElement();
                }

                QGraphicsPolygonItem *pPolygon = m_MainScene->addPolygon(polygon,QPen(myColor));
                if(pPolygon)
                {
                    pPolygon->setData(FLAGS_VISIBLE,nVisible);
                    pPolygon->setData(FLAGS_KEY,strKey); // 标示符
                    pPolygon->setData(FLAGS_TYPE,nType); // 类型：矩形还是圆
                    pPolygon->setData(FLAGS_X,fCx); //x
                    pPolygon->setData(FLAGS_Y,fCy); //y
                    pPolygon->setData(FLAGS_W,fCw); //w
                    pPolygon->setData(FLAGS_H,fCh); //h
                }
            }
        }

        e = e.nextSiblingElement();
    }

    m_bIsModify = true;

    return true;
}

bool QGraphicsWindow::SaveXml(QString fileName)
{
    QDomDocument doc;
    //instruction = doc.createProcessingInstruction("xml","version="1.0" encoding="UTF-8"");
    //doc.appendChild(instruction);

    QDomElement root = doc.createElement("root");
	root.setAttribute("color",m_ColorBtn->palette().background().color().name());
    doc.appendChild(root);

    QList<QGraphicsItem *> itemList = m_MainScene->items();
    for(int i=0;i<itemList.count();++i)
    {
        QGraphicsItem * item = itemList.at(i);
        if(item && item != m_MainImage)
        {
            int type = 0;
            int visible = 0;
            QString strKey;
            float px,py,pw,ph;

            visible = item->data(FLAGS_VISIBLE).toInt();
            strKey = item->data(FLAGS_KEY).toString();
            type = item->data(FLAGS_TYPE).toInt();
            px = item->data(FLAGS_X).toFloat();
            py = item->data(FLAGS_Y).toFloat();
            pw = item->data(FLAGS_W).toFloat();
            ph = item->data(FLAGS_H).toFloat();

            QDomElement element = doc.createElement("item");
            element.setAttribute(tr("VISIBLE"),tr("%1").arg(visible));
            element.setAttribute(tr("KEY"),tr("%1").arg(strKey));
            element.setAttribute(tr("TYPE"),tr("%1").arg(type));
            element.setAttribute(tr("CX"),tr("%1").arg(px));
            element.setAttribute(tr("CY"),tr("%1").arg(py));
            element.setAttribute(tr("CW"),tr("%1").arg(pw));
            element.setAttribute(tr("CH"),tr("%1").arg(ph));
            root.appendChild(element);

            if(type == QMyView::ITEM_POLYGON)
            {
                QRectF parentRect = m_MainImage->sceneBoundingRect();

                QGraphicsPolygonItem *polygon = (QGraphicsPolygonItem *)item;
                QPolygonF poly = polygon->polygon();
                foreach (QPointF point, poly)
                {
                    QDomElement child = doc.createElement("point");
                    child.setAttribute(tr("CX"),tr("%1").arg(point.x()/parentRect.width()));
                    child.setAttribute(tr("CY"),tr("%1").arg(point.y()/parentRect.height()));
                    element.appendChild(child);
                }
            }
        }
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
    return false;

    QTextStream out(&file);
    out.setCodec("UTF-8");
    doc.save(out,4,QDomNode::EncodingFromTextStream);
    file.close();

    return true;
}

void QGraphicsWindow::on_item_changed(int index,QString value )
{
    if(index == 1)
        m_MainView->m_FocusItem->setData(FLAGS_KEY,value);
}

void QGraphicsWindow::on_action_view3_triggered()
{
    if(m_pPropertyDlg->isHidden())
        m_pPropertyDlg->show();
}

void QGraphicsWindow::on_action_view1_triggered()
{
     if(ui->action_view1->isChecked())
     {
         m_pToolbar->show();
     }
     else
     {
         m_pToolbar->hide();
     }
}

void QGraphicsWindow::on_action_view2_triggered()
{
    if(ui->action_view2->isChecked())
    {
        m_pDrawToolbar->show();
    }
    else
    {
        m_pDrawToolbar->hide();
    }
}

void QGraphicsWindow::on_action_zoomin_triggered()
{
    if(m_MainImage)
        m_MainView->scale(1.1,1.1);
}

void QGraphicsWindow::on_action_zoomout_triggered()
{
    if(m_MainImage)
        m_MainView->scale(0.9,0.9);
}

void QGraphicsWindow::keyPressEvent(QKeyEvent *event)
{
	/*if(event->modifiers() == Qt::ControlModifier && 
		(m_MainView->m_Mode == QMyView::DRAW_MODE_RECT 
		||m_MainView->m_Mode == QMyView::DRAW_MODE_CIRCLE 
		||m_MainView->m_Mode == QMyView::DRAW_MODE_POLYGON ))
	{
		if(m_MainView)
			m_MainView->m_bIsCtrlPress = true;
		setCursor(QCursor(Qt::CrossCursor));
	}*/
}

void QGraphicsWindow::keyReleaseEvent(QKeyEvent *event)
{
	/*if(event->modifiers() != Qt::ControlModifier&& 
		(m_MainView->m_Mode == QMyView::DRAW_MODE_RECT 
		||m_MainView->m_Mode == QMyView::DRAW_MODE_CIRCLE 
		||m_MainView->m_Mode == QMyView::DRAW_MODE_POLYGON ))
	{
		if(m_MainView)
			m_MainView->m_bIsCtrlPress = false;
		setCursor(QCursor(Qt::ArrowCursor));
	}*/
}

#include <QColorDialog>
#include <QPalette>
void QGraphicsWindow::on_color_puton_triggered()
{
	QColor clr = QColorDialog::getColor();
	m_ColorBtn->setStyleSheet(QString("background-color:%1").arg(clr.name()));

	m_MainView->changePenColor(clr);

	QList<QGraphicsItem*> itemList = m_MainScene->items();
	foreach(QGraphicsItem *item,itemList)
	{
		if(item != m_MainImage)
		{
			int type = item->data(FLAGS_TYPE).toInt();
			if(type == QMyView::ITEM_RECT)
			{
				QGraphicsRectItem *rect = (QGraphicsRectItem*)item;
				rect->setPen(QPen(clr));
			}
			else if(type == QMyView::ITEM_CIRCLE)
			{
				QGraphicsEllipseItem *ellipse = (QGraphicsEllipseItem*)item;
				ellipse->setPen(QPen(clr));
			}
			else if(type == QMyView::ITEM_POLYGON)
			{
				QGraphicsPolygonItem *polygon = (QGraphicsPolygonItem*)item;
				polygon->setPen(QPen(clr));
			}
		}
	}
}
