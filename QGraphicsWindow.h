#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDockWidget>
#include <QWheelEvent>
#include "qmyview.h"
#include <QPushButton>

namespace Ui {
class QGraphicsWindow;
}

class QGraphicsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QGraphicsWindow(QWidget *parent = 0);
    ~QGraphicsWindow();

    bool m_bIsModify;
    QMyView *m_MainView;
    QGraphicsScene *m_MainScene;
    QGraphicsItem *m_MainImage;
    QGraphicsItem *m_SelectItem;
    QDockWidget *m_pPropertyDlg;
    QToolBar *m_pDrawToolbar;
    QToolBar *m_pToolbar;

    bool OpenXml(QString fileName);
    bool SaveXml(QString fileName);

protected:
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);

private slots:
    void on_action_open_triggered();

    void on_action_save_triggered();

    void on_action_saveAs_triggered();

    void on_action_rect_triggered();

    void on_action_circle_triggered();

    void on_action_move_triggered();

    void on_action_select_triggered();

    void on_action_close_triggered();

    void on_action_scale1_triggered();

    void on_action_zoomin_triggered();

    void on_action_zoomout_triggered();

    void on_action_load_triggered();

    void on_item_changed(int index,QString value );

    void on_action_view3_triggered();

    void on_action_view1_triggered();

    void on_action_view2_triggered();

    void on_action_polygon_triggered();

	void on_color_puton_triggered();

private:

    void InitToolbar();

    Ui::QGraphicsWindow *ui;

	QPushButton *m_ColorBtn;

    QString m_XmlFileName;
    QString m_ImageName;

};

#endif // MAINWINDOW_H
