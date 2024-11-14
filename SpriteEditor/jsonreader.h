#ifndef JSONREADER_H
#define JSONREADER_H

#include <QPixmap>
#include <QString>
#include "canvas.h"
#include "background.h"

/**
 * @class JsonReader
 * @brief A class for saving and loading sprite editor projects as JSON files.
 *
 * This class provides methods to save and load sprite project data, including
 * multiple frames stored as QPixmap objects and a scale value to define canvas drawing size.
 *
 * Key features:
 * - Saves a list of frames (QPixmaps) in a project to a JSON file using base64 encoding.
 * - Loads frames from a JSON file into a project by decoding base64 strings into QPixmap objects.
 * - Includes the project scale as part of the saved and loaded JSON data.
 *
 * @note This class operates with static methods only and does not require instantiation.
 */
class JsonReader
{
public:
    JsonReader();

    /**
     * @brief Saves the current frames and canvas scale to a JSON file.
     *
     * Encodes each frame in the provided list of QPixmaps as a base64 string, storing
     * these strings and the current canvas scale in a JSON file at the specified path.
     *
     * @param pixmaps List of QPixmap pointers representing project frames.
     * @param canvas Pointer to the Canvas containing the scale value to save.
     * @param filePath File path where the JSON data will be saved.
     * @return True if the data was saved successfully, false otherwise.
     */
    static bool savePixmapsToJson(QList<QPixmap*> pixmaps, Canvas* canvas, const QString &filePath);

    /**
     * @brief Loads frames and canvas scale from a JSON file into the project.
     *
     * Reads the JSON data from the specified file, decoding base64-encoded image data
     * into QPixmap objects to populate the project frame list. Also sets the scale on
     * the canvas based on the value stored in the file.
     *
     * @param pixmaps Reference to a list of QPixmap pointers to populate with loaded frames.
     * @param canvas Pointer to the Canvas on which to set the loaded scale value.
     * @param filePath File path to load the JSON data from.
     * @return True if the data was loaded successfully, false otherwise.
     */
    static bool loadPixmapsFromJson(QList<QPixmap*>& pixmaps, Canvas* canvas, Background* background, const QString &filePath);

};

#endif // JSONREADER_H
