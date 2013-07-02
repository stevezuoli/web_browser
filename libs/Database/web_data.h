#ifndef WEB_DATA_H_
#define WEB_DATA_H_

#include "Base/base.h"
#include <QtCore/QtCore>
#include <QtSql/QtSql>

namespace web_database
{

class WebData
{
public:
    WebData();
    ~WebData();

public:
    bool load(QVariantList& configuration);
    bool save(const QVariantList& configuration);
    void clear();

    void setCapacity(int max_num) { capacity_ = max_num; }
    int  capacity() { return capacity_; }

private:
    bool open(const QString& name);
    bool close();
    void makeSureTableExist(QSqlDatabase& database);
    bool clear(QSqlDatabase& database);

private:
    scoped_ptr<QSqlDatabase> database_;
    int capacity_;
};


class WebThumbnail : public QMap<QString, QVariant>
{
public:
    WebThumbnail();
    WebThumbnail( const QMap<QString, QVariant>& right );
    ~WebThumbnail();

public:
    static QSize size();

    void toByteArray(QByteArray & data);
    void fromByteArray(QByteArray & data);

    const QString title() const;
    void setTitle(const QString & title);

    const QUrl url() const;
    void setUrl(const QUrl & u);

    QImage image() const;
    void setImage(const QImage & image);

    void updateAccessDateTime();
    QString accessDateTime() const;
    void setAccessDateTime(const QString & time);

    QString accessDate() const;
    QString accessTime() const;

    static bool dateTimeGreaterThan(const QVariant& lhs, const QVariant& rhs);
};

struct GreaterByAccessTime
{
    bool operator()( const QVariant& a, const QVariant& b) const;
};

};

#endif  // WEB_DATA_H_
