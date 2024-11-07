#ifndef JSONREADER_H
#define JSONREADER_H

#include <QPixmap>
#include <QString>

/*
Grace Stewart, McKenna Wilson, Calvin Hickey, Alex Buus
A8: Sprite Editor

This header file defines the json reader for saving and loading sprite editor projects.
*/

class JsonReader
{
public:
    JsonReader();

    /**
     * @brief Saves the pixmap data to a json file by converting the Pixmap data into base24.
     * @param QPixmap to save data from.
     * @param QString filepath to save to.
     */
    static bool savePixmapToJson(const QPixmap &pixmap, const QString &filePath);

    /**
     * @brief Loads a json file by converting the base24 data into a Pixmap
     * @param QPixmap to load data into.
     * @param QString filePath to load from.
     */
    static bool loadPixmapFromJson(QPixmap &pixmap, const QString &filePath);
};

#endif // JSONREADER_H
