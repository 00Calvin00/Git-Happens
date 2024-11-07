#include "jsonreader.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QByteArray>
#include <QBuffer>

JsonReader::JsonReader() {

}

bool JsonReader::savePixmapToJson(const QPixmap &pixmap, const QString &filePath)
{
    // Convert QPixmap to QImage to QByteArray
    QImage image = pixmap.toImage();
    QByteArray byteArray;
    QBuffer buffer(&byteArray);

    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");

    // Encode the QByteArray to base64 string
    QString base64String = QString::fromLatin1(byteArray.toBase64());

    // Create JSON object and add the base64 image data
    QJsonObject json;
    json["image_data"] = base64String;

    // Write JSON object to file
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


