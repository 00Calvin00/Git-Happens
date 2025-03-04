/* Reviewed by: Alex */

#include "jsonreader.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>
#include <QByteArray>
#include <QBuffer>

JsonReader::JsonReader()
{

}

bool JsonReader::savePixmapsToJson(QList<QPixmap*> pixmapList, Canvas* canvas, const QString& filePath)
{
    // Create a JSON object for saving multiple frames
    QJsonObject json;

    // Create a JSON array to store the frames
    QJsonArray framesArray;

    for (QPixmap* pixmap : pixmapList)
    {
        // Convert each QPixmap to a QImage
        QImage image = pixmap -> toImage();

        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");

        // Encode the QByteArray to base64
        QString base64String = QString::fromLatin1(byteArray.toBase64());

        // Add the base64 string to the frames array
        framesArray.append(base64String);
    }

    // Add the frames array to the JSON object
    json["frames"] = framesArray;

    // Add scale to the Json object
    json["scale"] = canvas->getScale();

    // Write the JSON object to file
    QJsonDocument jsonDoc(json);
    QFile jsonFile(filePath);

    if (!jsonFile.open(QIODevice::WriteOnly))
    {
        qWarning("Could not open file for writing.");
        return false;
    }

    jsonFile.write(jsonDoc.toJson(QJsonDocument::Indented));
    jsonFile.close();

    return true;
}

bool JsonReader::loadPixmapsFromJson(QList<QPixmap*>& pixmapList, Canvas* canvas, Background* background, const QString& filePath)
{
    QFile jsonFile(filePath);
    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Could not open file for reading:" << filePath;
        return false;
    }

    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();

    // Parse the JSON data
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull())
    {
        qWarning() << "Failed to parse JSON";
        return false;
    }

    // Extract the frames array from JSON
    QJsonObject jsonObject = jsonDoc.object();
    if (!jsonObject.contains("frames"))
    {
        qWarning() << "No frames data found in JSON";
        return false;
    }

    // Extract the scale
    if (jsonObject.contains("scale"))
    {
        int scale = jsonObject["scale"].toInt();
        canvas->setScale(scale);
        background->setScale(scale);
    }

    // Delete all existing pixmaps in the list before loading new ones
    qDeleteAll(pixmapList);
    pixmapList.clear();

    QJsonArray framesArray = jsonObject["frames"].toArray();
    for (const QJsonValue& value : framesArray)
    {
        QString base64String = value.toString();

        // Decode the base64 string
        QByteArray byteArray = QByteArray::fromBase64(base64String.toLatin1());

        // Load the QByteArray into a QImage
        QImage image;
        if (!image.loadFromData(byteArray, "PNG"))
        {
            qWarning() << "Failed to load image from data";
            return false;
        }

        // Convert the QImage to QPixmap and add to the list
        QPixmap* newMap = new QPixmap(QPixmap::fromImage(image));

        pixmapList.append(newMap);
    }

    return true;
}
