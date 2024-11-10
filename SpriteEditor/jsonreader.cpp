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

bool JsonReader::savePixmapsToJson(const QList<QPixmap>& pixmapList, const QString& filePath)
{
    // Create a JSON object for saving multiple frames
    QJsonObject json;

    // Create a JSON array to store the frames
    QJsonArray framesArray;

    for (const QPixmap& pixmap : pixmapList) {
        // Convert each QPixmap to a QImage
        QImage image = pixmap.toImage();
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

    // Write the JSON object to file
    QJsonDocument jsonDoc(json);
    QFile jsonFile(filePath);

    if (!jsonFile.open(QIODevice::WriteOnly)) {
        qWarning("Could not open file for writing.");
        return false;
    }

    jsonFile.write(jsonDoc.toJson(QJsonDocument::Indented));
    jsonFile.close();

     qDebug() << "Saved frames count: " << pixmapList.size();

    return true;
}



// bool JsonReader::savePixmapToJson(const QPixmap &pixmap, const QString &filePath)
// {
//     // Convert QPixmap to QImage to QByteArray
//     QImage image = pixmap.toImage();
//     QByteArray byteArray;
//     QBuffer buffer(&byteArray);

//     buffer.open(QIODevice::WriteOnly);
//     image.save(&buffer, "PNG");

//     // Encode the QByteArray to base64 string
//     QString base64String = QString::fromLatin1(byteArray.toBase64());

//     // Create JSON obj and add the base64 image data
//     QJsonObject json;
//     json["image_data"] = base64String;

//     // Write JSON obj to file
//     QJsonDocument jsonDoc(json);
//     QFile jsonFile(filePath);

//     // Catch any unwritable file error (file open in another window or file in read-only mode)
//     if (!jsonFile.open(QIODevice::WriteOnly))
//     {
//         qWarning("Could not open file for writing.");
//         return false;
//     }

//     jsonFile.write(jsonDoc.toJson(QJsonDocument::Indented));
//     jsonFile.close();

//     return true;
// }

bool JsonReader::loadPixmapsFromJson(QList<QPixmap>& pixmapList, const QString& filePath)
{
    QFile jsonFile(filePath);
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open file for reading:" << filePath;
        return false;
    }

    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();

    // Parse the JSON data
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull()) {
        qWarning() << "Failed to parse JSON";
        return false;
    }

    // Extract the frames array from JSON
    QJsonObject jsonObject = jsonDoc.object();
    if (!jsonObject.contains("frames")) {
        qWarning() << "No frames data found in JSON";
        return false;
    }

    QJsonArray framesArray = jsonObject["frames"].toArray();
    for (const QJsonValue& value : framesArray) {
        QString base64String = value.toString();

        // Decode the base64 string
        QByteArray byteArray = QByteArray::fromBase64(base64String.toLatin1());

        // Load the QByteArray into a QImage
        QImage image;
        if (!image.loadFromData(byteArray, "PNG")) {
            qWarning() << "Failed to load image from data";
            return false;
        }

        // Convert the QImage to QPixmap and add to the list
        pixmapList.append(QPixmap::fromImage(image));
    }

     qDebug() << "Loaded frames count: " << pixmapList.size();

    return true;
}



// bool JsonReader::loadPixmapFromJson(QPixmap &pixmap, const QString &filePath)
// {
//     // Open the JSON file
//     QFile jsonFile(filePath);
//     if (!jsonFile.open(QIODevice::ReadOnly))
//     {
//         qWarning() << "Could not open file for reading:" << filePath;
//         return false;
//     }

//     // Read the JSON content
//     QByteArray jsonData = jsonFile.readAll();
//     jsonFile.close();

//     // Parse the JSON data
//     QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
//     if (jsonDoc.isNull())
//     {
//         qWarning() << "Failed to parse JSON";
//         return false;
//     }

//     // Extract the base64 image data from JSON
//     QJsonObject jsonObject = jsonDoc.object();
//     if (!jsonObject.contains("image_data"))
//     {
//         qWarning() << "No image data found in JSON";
//         return false;
//     }
//     QString base64String = jsonObject["image_data"].toString();

//     // Decode the base64 string to QByteArray
//     QByteArray byteArray = QByteArray::fromBase64(base64String.toLatin1());

//     // Load the QByteArray into a QImage
//     QImage image;
//     if (!image.loadFromData(byteArray, "PNG"))
//     {
//         qWarning() << "Failed to load image from data";
//         return false;
//     }

//     // Convert the QImage to QPixmap
//     pixmap = QPixmap::fromImage(image);

//     return true;
// }


