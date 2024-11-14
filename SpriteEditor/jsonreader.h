#ifndef JSONREADER_H
#define JSONREADER_H

#include <QPixmap>
#include <QString>
#include "canvas.h"

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

    /**
     * @brief Retrieves the actual current frames from the PixmapList
     * @return The current QPixmap representing the canvas.
     */
    static bool savePixmapsToJson(QList<QPixmap*> pixmaps, Canvas* canvas, const QString &filePath);

    /**
     * @brief Retrieves the actual current frames from the PixmapList
     * @return The current QPixmap representing the canvas.
     */
    static bool loadPixmapsFromJson(QList<QPixmap*>& pixmaps, Canvas* canvas, const QString &filePath);

};

#endif // JSONREADER_H
