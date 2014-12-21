#ifndef QMYVIEW_H
#define QMYVIEW_H

#include <QGraphicsView>


class QMyView : public QGraphicsView
{
   // Q_OBJECT

public:
    explicit QMyView(QWidget *parent = 0);
    ~QMyView();

    enum ITEM_TYPE
    {
        ITEM_RECT,
        ITEM_CIRCLE,
        ITEM_POLYGON
    };

    enum DRAW_MODE{
        DRAW_MODE_SELECT,
        DRAW_MODE_MOVE,
        DRAW_MODE_SCALE,
        DRAW_MODE_RECT,
        DRAW_MODE_CIRCLE,
        DRAW_MODE_POLYGON
    }m_Mode;

	bool m_bIsCtrlPress;

	QGraphicsItem *m_NewItem;
    QGraphicsItem *m_FocusItem; //当前选区
    QGraphicsItem *m_LastFocusItem; //上一次选中选区
    QGraphicsItem *m_CopyItem;// 复制选区

    virtual void mouseMoveEvent(QMouseEvent *event);
	void changePenColor(QColor clr);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
	virtual void wheelEvent(QWheelEvent *evt);

private:
    QPointF m_MousePressPos;
    QRectF m_OldScale;
    bool m_bIsMousePress;
    float m_fOffsetX;
    float m_fOffsetY;
    QList<QGraphicsLineItem*> m_LineItemList;
    QList<QPointF> m_PointArray;
    QPolygonF m_Polygon;
	QColor m_Color;
};

#endif // QMYVIEW_H
