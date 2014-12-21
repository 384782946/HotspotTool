#include "qmyview.h"
#include <QMouseEvent>
#include <QDebug>
#include "QGraphicsWindow.h"
#include "propertydlg.h"

QMyView::QMyView(QWidget * parent):QGraphicsView(parent)
{
	m_bIsCtrlPress = false;
    m_Mode = DRAW_MODE_SELECT;
    m_FocusItem = NULL;
    m_LastFocusItem = NULL;
	m_NewItem = NULL;

    setMouseTracking(true);
}

QMyView::~QMyView()
{

}

void QMyView::mousePressEvent(QMouseEvent *event)
{
    event->accept();

    m_MousePressPos = mapToScene(event->pos());
    m_bIsMousePress = true;

    if (QGraphicsItem *item = itemAt(event->pos()))
    {
        QGraphicsWindow *mainWindow = (QGraphicsWindow *)parent();
        if(mainWindow && event->button() == Qt::RightButton && m_Mode == DRAW_MODE_POLYGON)
        {
            foreach (QGraphicsLineItem *item, m_LineItemList) {
                mainWindow->m_MainScene->removeItem(item);
            }

            m_LineItemList.clear();
            m_PointArray.clear();
        }

        if(mainWindow && event->button() == Qt::LeftButton)
        {
            if(item != mainWindow->m_MainImage)
                m_FocusItem = item;
            else
                m_FocusItem = NULL;

            if(m_Mode == DRAW_MODE_MOVE && item != mainWindow->m_MainImage)
            {
                setCursor(QCursor(Qt::SizeAllCursor));

                QPointF curPos = item->pos();
                QPointF mousePos = mapToScene(event->pos());
                m_fOffsetX = mousePos.x() - curPos.x();
                m_fOffsetY = mousePos.y() - curPos.y();
            }
            else if(m_Mode == DRAW_MODE_SCALE && item != mainWindow->m_MainImage)
            {
                setCursor(QCursor(Qt::SizeFDiagCursor));

                if(m_FocusItem->data(FLAGS_TYPE).toInt()==0)
                {
                    QGraphicsRectItem *item = (QGraphicsRectItem *)m_FocusItem;
                    m_OldScale = item->rect();
                }
                else
                {
                    QGraphicsEllipseItem *item = (QGraphicsEllipseItem *)m_FocusItem;
                    m_OldScale = item->rect();
                }

                mainWindow->m_bIsModify = true;
            }
			else if(m_Mode == DRAW_MODE_RECT && m_bIsCtrlPress &&item == mainWindow->m_MainImage)
			{
				m_NewItem = mainWindow->m_MainScene->addRect(m_MousePressPos.x(),m_MousePressPos.y(),5,5,QPen(m_Color));
				m_FocusItem = m_NewItem;

				QRectF itemRect = m_FocusItem->sceneBoundingRect();
				QRectF prentRect = mainWindow->m_MainImage->sceneBoundingRect();
				float px,py,pw,ph;
				px = (itemRect.x()-prentRect.x())/prentRect.width();
				py = (itemRect.y()-prentRect.y())/prentRect.height();
				pw = itemRect.width()/prentRect.width();
				ph = itemRect.height()/prentRect.height();

				QList<QGraphicsItem *> itemList = mainWindow->m_MainScene->items();

				m_FocusItem->setData(FLAGS_VISIBLE,1);
				m_FocusItem->setData(FLAGS_KEY,tr("item_%1").arg(itemList.count()-1)); // 标示符
				m_FocusItem->setData(FLAGS_TYPE,ITEM_RECT); // 类型：矩形还是圆
				m_FocusItem->setData(FLAGS_X,px); //x
				m_FocusItem->setData(FLAGS_Y,py); //y
				m_FocusItem->setData(FLAGS_W,pw); //w
				m_FocusItem->setData(FLAGS_H,ph); //h

				mainWindow->m_bIsModify = true;
			}
			else if(m_Mode == DRAW_MODE_CIRCLE  && m_bIsCtrlPress && item == mainWindow->m_MainImage)
			{
				m_NewItem = mainWindow->m_MainScene->addEllipse(m_MousePressPos.x(),m_MousePressPos.y(),5,5,QPen(m_Color));
				m_FocusItem = m_NewItem;

				QRectF itemRect = m_FocusItem->sceneBoundingRect();
				QRectF prentRect = mainWindow->m_MainImage->sceneBoundingRect();
				float px,py,pw,ph;
				px = (itemRect.x()-prentRect.x())/prentRect.width();
				py = (itemRect.y()-prentRect.y())/prentRect.height();
				pw = itemRect.width()/prentRect.width();
				ph = itemRect.height()/prentRect.height();

				QList<QGraphicsItem *> itemList = mainWindow->m_MainScene->items();
				m_FocusItem->setData(FLAGS_VISIBLE,1);
				m_FocusItem->setData(FLAGS_KEY,tr("item_%1").arg(itemList.count()-1)); // 标示符
				m_FocusItem->setData(FLAGS_TYPE,ITEM_CIRCLE); // 类型：矩形还是圆
				m_FocusItem->setData(FLAGS_X,px); //x
				m_FocusItem->setData(FLAGS_Y,py); //y
				m_FocusItem->setData(FLAGS_W,pw); //w
				m_FocusItem->setData(FLAGS_H,ph); //h

				mainWindow->m_bIsModify = true;
			}
			else if(m_Mode == DRAW_MODE_POLYGON && m_bIsCtrlPress )
			{
				if(m_PointArray.count() == 0)
				{
					m_PointArray.append(m_MousePressPos);
					QGraphicsLineItem *lineItem = mainWindow->m_MainScene->addLine(QLine(m_MousePressPos.toPoint(),m_MousePressPos.toPoint()),QPen(m_Color));
					m_LineItemList<<lineItem;
					m_FocusItem = NULL;
				}
				else
				{
					QPointF firstPoint = m_PointArray.first();
					QPointF lastPoint = m_PointArray.last();

					if(abs(m_MousePressPos.x()-firstPoint.x())<4 && abs(m_MousePressPos.y()-firstPoint.y())<4)
					{
						m_Polygon.clear();
						foreach (QPointF point, m_PointArray) {
							m_Polygon<<point;
						}

						m_NewItem = mainWindow->m_MainScene->addPolygon(m_Polygon,QPen(m_Color));
						m_FocusItem = m_NewItem;

						foreach (QGraphicsLineItem *item, m_LineItemList) {
							mainWindow->m_MainScene->removeItem(item);
						}

						m_LineItemList.clear();
						m_PointArray.clear();

						QRectF itemRect = m_FocusItem->sceneBoundingRect();
						QRectF prentRect = mainWindow->m_MainImage->sceneBoundingRect();
						float px,py,pw,ph;
						px = (itemRect.x()-prentRect.x())/prentRect.width();
						py = (itemRect.y()-prentRect.y())/prentRect.height();
						pw = itemRect.width()/prentRect.width();
						ph = itemRect.height()/prentRect.height();

						QList<QGraphicsItem *> itemList = mainWindow->m_MainScene->items();
						m_FocusItem->setData(FLAGS_VISIBLE,1);
						m_FocusItem->setData(FLAGS_KEY,tr("item_%1").arg(itemList.count()-1)); // 标示符
						m_FocusItem->setData(FLAGS_TYPE,ITEM_POLYGON); // 类型：矩形还是圆
						m_FocusItem->setData(FLAGS_X,px); //x
						m_FocusItem->setData(FLAGS_Y,py); //y
						m_FocusItem->setData(FLAGS_W,pw); //w
						m_FocusItem->setData(FLAGS_H,ph); //h

						mainWindow->m_bIsModify = true;
					}
					else
					{
						QGraphicsLineItem *lastLine = m_LineItemList.last();
						lastLine->setLine(QLine(lastPoint.toPoint(),m_MousePressPos.toPoint()));
						QGraphicsLineItem *lineItem = mainWindow->m_MainScene->addLine(QLine(m_MousePressPos.toPoint(),m_MousePressPos.toPoint()));
						m_LineItemList<<lineItem;
						m_PointArray.append(m_MousePressPos);
						m_FocusItem = NULL;
					}
				}
			}

            if(m_FocusItem)
            {
                int type = m_FocusItem->data(FLAGS_TYPE).toInt();
                if(type == ITEM_RECT)
                {
                    QGraphicsRectItem *rect = (QGraphicsRectItem*)m_FocusItem;
                    rect->setPen(QPen(Qt::red));
                }
                else if(type == ITEM_CIRCLE)
                {
                    QGraphicsEllipseItem *ellipse = (QGraphicsEllipseItem*)m_FocusItem;
                    ellipse->setPen(QPen(Qt::red));
                }
                else if(type == ITEM_POLYGON)
                {
                    QGraphicsPolygonItem *polygon = (QGraphicsPolygonItem*)m_FocusItem;
                    polygon->setPen(QPen(Qt::red));
                }
            }

            if(m_LastFocusItem && m_LastFocusItem != m_FocusItem)
            {
                int type = m_LastFocusItem->data(FLAGS_TYPE).toInt();
                if(type == ITEM_RECT)
                {
                    QGraphicsRectItem *rect = (QGraphicsRectItem*)m_LastFocusItem;
                    rect->setPen(QPen(m_Color));
                }
                else if(type == ITEM_CIRCLE)
                {
                    QGraphicsEllipseItem *ellipse = (QGraphicsEllipseItem*)m_LastFocusItem;
                    ellipse->setPen(QPen(m_Color));
                }
                else if(type == ITEM_POLYGON)
                {
                    QGraphicsPolygonItem *polygon = (QGraphicsPolygonItem*)m_LastFocusItem;
                    polygon->setPen(QPen(m_Color));
                }
            }

            m_LastFocusItem = m_FocusItem;

            PropertyDlg *propertyDlg = ( PropertyDlg *)mainWindow->m_pPropertyDlg->widget();
            if(propertyDlg)
            {
                if(item == mainWindow->m_MainImage)
                   propertyDlg->InitProperty(item,NULL);
                else if(m_FocusItem)
                   propertyDlg->InitProperty(m_FocusItem,mainWindow->m_MainImage);
            }
        }
    }
    else
    {
    }
}

void QMyView::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
    QPointF currentPos = mapToScene( event->pos());

    QGraphicsWindow *mainWindow = (QGraphicsWindow *)parent();
    if(mainWindow)
    {
        if(m_Mode == DRAW_MODE_POLYGON && m_PointArray.count()>0)
        {
            QPointF lastPiont = m_PointArray.last();
            QGraphicsLineItem *lastLine = m_LineItemList.last();
            lastLine->setLine(QLine(lastPiont.toPoint(),currentPos.toPoint()));

            if(m_PointArray.count()>2)
            {
                QPointF firstPoint = m_PointArray.first();
                QPointF lastPoint = m_PointArray.last();

                if(abs(currentPos.x()-firstPoint.x())<4 && abs(currentPos.y()-firstPoint.y())<4)
                {
                    setCursor(QCursor(Qt::PointingHandCursor));
                }
                else
                {
                    setCursor(QCursor(Qt::ArrowCursor));
                }
            }
            else
            {
                setCursor(QCursor(Qt::ArrowCursor));
            }
        }

        if(m_bIsMousePress && m_FocusItem)
        {
            if(m_Mode == DRAW_MODE_MOVE)
            {
                float dX,dY;
                dX = currentPos.x() - m_fOffsetX;
                dY = currentPos.y() - m_fOffsetY;
                m_FocusItem->setPos(dX,dY);

                mainWindow->m_bIsModify = true;
            }
            else if(m_Mode == DRAW_MODE_SCALE)
            {
                if(dynamic_cast<QGraphicsPolygonItem*>(m_FocusItem))
                    return;

                float x = m_OldScale.width() + currentPos.x() - m_MousePressPos.x();
                float y = m_OldScale.height() + currentPos.y() - m_MousePressPos.y();

                if(x<5)
                {
                    x = 5;
                }

                if(y<5)
                {
                    y = 5;
                }

                if(m_FocusItem->data(FLAGS_TYPE).toInt()==0)
                {
                    QGraphicsRectItem *item = (QGraphicsRectItem *)m_FocusItem;
                    QRectF rct = item->rect();
                    rct.setWidth(x);
                    rct.setHeight(y);
                    item->setRect(rct);
                }
                else
                {
                    QGraphicsEllipseItem *item = (QGraphicsEllipseItem *)m_FocusItem;
                    QRectF rct = item->rect();
                    rct.setWidth(x);
                    rct.setHeight(y);
                    item->setRect(rct);
                }

                mainWindow->m_bIsModify = true;
            }
            else if(m_Mode == DRAW_MODE_RECT && m_FocusItem == m_NewItem)
            {
                float offsetX = currentPos.x() - m_MousePressPos.x();
                float offsetY = currentPos.y() - m_MousePressPos.y();

                if(offsetX < 5)
                {
                    offsetX = 5;
                }
                if(offsetY < 5)
                {
                    offsetY = 5;
                }

                QGraphicsRectItem *item = (QGraphicsRectItem *)m_FocusItem;
                QRectF rct = item->rect();
                rct.setWidth(offsetX);
                rct.setHeight(offsetY);
                item->setRect(rct);

                mainWindow->m_bIsModify = true;
            }
            else if(m_Mode == DRAW_MODE_CIRCLE && m_FocusItem == m_NewItem)
            {
                float offsetX = currentPos.x() - m_MousePressPos.x();
                float offsetY = currentPos.y() - m_MousePressPos.y();

                if(offsetX < 5)
                {
                    offsetX = 5;
                }
                if(offsetY < 5)
                {
                    offsetY = 5;
                }

                QGraphicsEllipseItem *item = (QGraphicsEllipseItem *)m_FocusItem;
                QRectF rct = item->rect();
                rct.setWidth(offsetX);
                rct.setHeight(offsetY);
                item->setRect(rct);

                mainWindow->m_bIsModify = true;
            }
        }
    }
}

void QMyView::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    m_bIsMousePress = false;
    setCursor(QCursor(Qt::ArrowCursor));

    QGraphicsWindow *mainWindow = (QGraphicsWindow *)parent();
    if(mainWindow )
    {
		if(m_NewItem)
		{
			m_NewItem = NULL;
		}

        if(m_FocusItem)
        {
            QRectF itemRect = m_FocusItem->sceneBoundingRect();
            QRectF prentRect = mainWindow->m_MainImage->sceneBoundingRect();
            float px,py,pw,ph;
            px = (itemRect.x()-prentRect.x())/prentRect.width();
            py = (itemRect.y()-prentRect.y())/prentRect.height();
            pw = itemRect.width()/prentRect.width();
            ph = itemRect.height()/prentRect.height();

            m_FocusItem->setData(FLAGS_X,px); //x
            m_FocusItem->setData(FLAGS_Y,py); //y
            m_FocusItem->setData(FLAGS_W,pw); //w
            m_FocusItem->setData(FLAGS_H,ph); //h

            PropertyDlg *propertyDlg = ( PropertyDlg *)mainWindow->m_pPropertyDlg->widget();
            if(propertyDlg)
            {
                propertyDlg->InitProperty(m_FocusItem,(mainWindow->m_MainImage == m_FocusItem)?NULL:mainWindow->m_MainImage);
            }
        }
    }
}

void QMyView::keyPressEvent(QKeyEvent *event)
{
    QGraphicsWindow *mainWindow = (QGraphicsWindow *)parent();
    if(!mainWindow) return;

    int nKey = event->key();
    if(nKey == Qt::Key_Delete && m_FocusItem)
    {
        mainWindow->m_MainScene->removeItem(m_FocusItem);
        mainWindow->m_bIsModify = true;
        m_FocusItem = NULL;
        m_LastFocusItem = NULL;
    }
    else if(event->modifiers() == Qt::ControlModifier && nKey == Qt::Key_C)
    {
        if(mainWindow->m_MainImage && m_FocusItem)
        {
            m_CopyItem = m_FocusItem;
        }
    }
    else if(event->modifiers() == Qt::ControlModifier && nKey == Qt::Key_V)
    {
        if(m_CopyItem && mainWindow->m_MainImage)
        {
            int nType = m_CopyItem->data(FLAGS_TYPE).toInt();
            if( nType == 0)
            {
                QGraphicsRectItem *pRect = mainWindow->m_MainScene->addRect(m_CopyItem->sceneBoundingRect());
                for(int i=0;i<FLAGS_END;++i)
                {
                    pRect->setData(i,m_CopyItem->data(i));
                }
            }
            else if(nType == 1)
            {
                QGraphicsEllipseItem *pEllipse = mainWindow->m_MainScene->addEllipse(m_CopyItem->sceneBoundingRect());
                for(int i=0;i<FLAGS_END;++i)
                {
                    pEllipse->setData(i,m_CopyItem->data(i));
                }
            }
        }
    }
    else if(nKey == Qt::Key_Up && m_FocusItem)
    {
        QPointF pos = m_FocusItem->pos();
        m_FocusItem->setPos(pos.x(),pos.y()-1);
    }
    else if(nKey == Qt::Key_Down && m_FocusItem)
    {
        QPointF pos = m_FocusItem->pos();
        m_FocusItem->setPos(pos.x(),pos.y()+1);
    }
    else if(nKey == Qt::Key_Right && m_FocusItem)
    {
        QPointF pos = m_FocusItem->pos();
        m_FocusItem->setPos(pos.x()+1,pos.y());
    }
    else if(nKey == Qt::Key_Left && m_FocusItem)
    {
        QPointF pos = m_FocusItem->pos();
        m_FocusItem->setPos(pos.x()-1,pos.y());
    }
	else if(event->modifiers() == Qt::ControlModifier /*&& 
		(m_Mode == DRAW_MODE_RECT 
		||m_Mode == DRAW_MODE_CIRCLE )*/)
	{
		m_bIsCtrlPress = true;
		setCursor(QCursor(Qt::CrossCursor));
	}
}

void QMyView::keyReleaseEvent(QKeyEvent *event)
{
	if(event->modifiers() != Qt::ControlModifier && m_bIsCtrlPress)
	{
		m_bIsCtrlPress = false;
		setCursor(QCursor(Qt::ArrowCursor));
	}
}

void QMyView::changePenColor(QColor clr)
{
	m_Color = clr;
}

void QMyView::wheelEvent(QWheelEvent *evt)
{
	if(evt->modifiers() != Qt::ControlModifier)
	{
		QGraphicsView::wheelEvent(evt);
	}
	else
	{
		int fact = evt->delta();

		if(fact >0)
			scale(1.05,1.05);
		else if(fact <0)
			scale(0.95,0.95);
	}
}
