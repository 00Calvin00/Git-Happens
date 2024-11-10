#ifndef JSONREADER_H
#define JSONREADER_H

#include <QPixmap>
#include <QString>

/**
 * @class JsonReader
 * @brief A class for saving and loading sprite editor projects using json files.
 *
 * Key features:
 * -
 *
 * @note
 */

class JsonReader
{
public:
    JsonReader();

    static bool savePixmapsToJson(QList<QPixmap*> pixmaps, const QString &filePath);

    static bool loadPixmapsFromJson(QList<QPixmap*> pixmaps, const QString &filePath);

};

#endif // JSONREADER_H
