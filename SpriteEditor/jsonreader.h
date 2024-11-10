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
 * @note The `Canvas` widget is designed to be used as part of a larger application
 * where it can be embedded in a layout or handled as a standalone widget. It is
 * particularly useful for applications that require pixel-based editing or simple
 * drawing functionality.
 */

class JsonReader
{
public:
    JsonReader();

    static bool savePixmapsToJson(const QList<QPixmap> &pixmaps, const QString &filePath);

    static bool loadPixmapsFromJson(QList<QPixmap> &pixmaps, const QString &filePath);

    // /**
    //  * @brief Saves the pixmap data to a json file by converting the Pixmap data into base24.
    //  * @param QPixmap to save data from.
    //  * @param QString filepath to save to.
    //  */
    // static bool savePixmapToJson(const QPixmap &pixmap, const QString &filePath);

    // /**
    //  * @brief Loads a json file by converting the base24 data into a Pixmap
    //  * @param QPixmap to load data into.
    //  * @param QString filePath to load from.
    //  */
    // static bool loadPixmapFromJson(QPixmap &pixmap, const QString &filePath);
};

#endif // JSONREADER_H
