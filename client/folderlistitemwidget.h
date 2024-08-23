#ifndef FOLDERLISTITEMWIDGET_H
#define FOLDERLISTITEMWIDGET_H

#include <QWidget>
#include <QString>

namespace Ui {
class FolderListItemWidget;
}

class FolderListItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FolderListItemWidget(int _type, QWidget *parent = nullptr);
    ~FolderListItemWidget();
    int getType();
    void init(QString name, QString path, qint64 size);
    void setName(QString name);
    void setIcon(QString path);
    void setSize(qint64 size);
    QString getFileName();
    qint64 getFileSize();

private:
    Ui::FolderListItemWidget *ui;

    int type; ///< 文件类型
    QString fileName;
    qint64 fileSize;
};

#endif // FOLDERLISTITEMWIDGET_H
