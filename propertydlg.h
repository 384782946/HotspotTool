#ifndef PROPERTYDLG_H
#define PROPERTYDLG_H

#include <QWidget>
#include <QTableWidget>
#include <QGraphicsItem>
#include <QMenu>

enum FLAGS
{
    FLAGS_VISIBLE,
    FLAGS_KEY,
    FLAGS_TYPE,
    FLAGS_X,
    FLAGS_Y,
    FLAGS_W,
    FLAGS_H,
    FLAGS_END
};

namespace Ui {
class PropertyDlg;
}

class PropertyDlg : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyDlg(QWidget *parent = 0);
    ~PropertyDlg();

    void InitProperty(QGraphicsItem *item,QGraphicsItem *parent = NULL);

protected:
   virtual QSize sizeHint() const;

signals:
    void itemChanged(int index,QString value );

private slots:
    void on_tableWidget_cellChanged(int row, int column);
    void on_visible_change(int index);

private:
    Ui::PropertyDlg *ui;
    QGraphicsItem *m_CurrentItem;
    QStringList m_TypeNameList;
};

#endif // PROPERTYDLG_H
