#ifndef JSONREADER_H
#define JSONREADER_H

#include <QPixmap>
#include <QString>

class JsonReader
{
public:
    JsonReader();

    static bool savePixmapToJson(const QPixmap &pixmap, const QString &filePath);

    // Load QPixmap data from a JSON file
    static QPixmap loadPixmapFromJson(const QString &filePath);
};

#endif // JSONREADER_H
