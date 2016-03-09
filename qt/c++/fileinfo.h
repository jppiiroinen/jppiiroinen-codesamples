/*****************************************************************************
THIS IS A CODE SAMPLE.

##############################
# (C) 2015 Juhapekka Piiroinen
# All Rights Reserved.
##############################
# Author(s):
#   Juhapekka Piiroinen <juhapekka.piiroinen@1337.fi>
##############################
******************************************************************************/
#ifndef FILEINFO_H
#define FILEINFO_H

#include <QtQml>
#include <QObject>
#include <QFileInfo>
#include <QList>
#include <QMimeDatabase>
#include <QStandardPaths>
#include <QIcon>

class FileInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ get_fileName NOTIFY fileChanged)
    Q_PROPERTY(QString filePath READ get_filePath NOTIFY fileChanged)
    Q_PROPERTY(QString absoluteFilePath READ get_absolute_filepath NOTIFY fileChanged)
    Q_PROPERTY(QString suffix READ get_fileType NOTIFY fileChanged)
    Q_PROPERTY(QString mimeType READ get_mimeType NOTIFY fileChanged)
    Q_PROPERTY(QString ownerUser READ get_owner_user NOTIFY fileChanged)
    Q_PROPERTY(QString ownerGroup READ get_owner_group NOTIFY fileChanged)
    Q_PROPERTY(QDateTime modified READ get_modified NOTIFY fileChanged)
    Q_PROPERTY(QDate modified_date READ get_modified_date NOTIFY fileChanged)
    Q_PROPERTY(bool isDir READ is_directory NOTIFY fileChanged)
    Q_PROPERTY(bool isFile READ is_file NOTIFY fileChanged)
    Q_PROPERTY(QIcon icon READ get_icon NOTIFY fileChanged)
    Q_PROPERTY(bool isHidden READ is_hidden NOTIFY fileChanged)
    Q_PROPERTY(bool isExecutable READ is_executable NOTIFY fileChanged)
    Q_PROPERTY(bool isWritable READ is_writable NOTIFY fileChanged)
    Q_PROPERTY(bool isReadable READ is_readable NOTIFY fileChanged)
    Q_PROPERTY(qint64 size READ get_size NOTIFY fileChanged)
    Q_PROPERTY(QString icon_name READ get_icon_name NOTIFY fileChanged)
    Q_PROPERTY(QString displayName READ get_displayName NOTIFY fileChanged)
    Q_PROPERTY(QStringList emblems READ get_emblems NOTIFY fileChanged)
    Q_PROPERTY(bool isSymLink READ is_symbolic_link NOTIFY fileChanged)
    Q_PROPERTY(QString symLinkTarget READ get_symbolicLinkTarget NOTIFY fileChanged)
    Q_PROPERTY(bool isSpecialPath READ is_special_path NOTIFY fileChanged)

public:
    FileInfo(QObject* parent = 0);
    FileInfo(QFileInfo fileInfo, qulonglong dirSize = 0, QObject *parent = 0);

    Q_INVOKABLE void execute() {
        QProcess::startDetached(m_fileInfo.absoluteFilePath());
    }

    Q_INVOKABLE static QString get_absolute_filepath(QString path) {
        return QFileInfo(path).absoluteFilePath();
    }

    Q_INVOKABLE static QString get_displayName(QString path) {
        QFileInfo fileInfo(path);
        if (fileInfo.isDir()) {
            QString retval = "";
           if (fileInfo.isRoot()) {
               retval = fileInfo.fileName();
           } else {
               QString fileName = fileInfo.absoluteFilePath();

               if (QStandardPaths::standardLocations(QStandardPaths::HomeLocation).contains(fileName)) {
                    retval = QStandardPaths::displayName(QStandardPaths::HomeLocation);
               } else if (QStandardPaths::standardLocations(QStandardPaths::RuntimeLocation).contains(fileName)) {
                   retval = QStandardPaths::displayName(QStandardPaths::RuntimeLocation);
               } else if (QStandardPaths::standardLocations(QStandardPaths::TempLocation).contains(fileName)) {
                   retval = QStandardPaths::displayName(QStandardPaths::TempLocation);
               } else if (QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation).contains(fileName)) {
                   retval = QStandardPaths::displayName(QStandardPaths::ApplicationsLocation);
               } else if (QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).contains(fileName)) {
                   retval = QStandardPaths::displayName(QStandardPaths::PicturesLocation);
               } else if (QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).contains(fileName)) {
                   retval = QStandardPaths::displayName(QStandardPaths::MoviesLocation);
               } else if (QStandardPaths::standardLocations(QStandardPaths::MusicLocation).contains(fileName)) {
                   retval = QStandardPaths::displayName(QStandardPaths::MusicLocation);
               } else if (QStandardPaths::standardLocations(QStandardPaths::CacheLocation).contains(fileName)) {
                   retval = QStandardPaths::displayName(QStandardPaths::CacheLocation);
              } else if (QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).contains(fileName)) {
                   retval = QStandardPaths::displayName(QStandardPaths::ConfigLocation);
              } else if (QStandardPaths::standardLocations(QStandardPaths::DataLocation).contains(fileName)) {
                   retval = QStandardPaths::displayName(QStandardPaths::DataLocation);
              } else if (QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).contains(fileName)) {
                   retval = QStandardPaths::displayName(QStandardPaths::DesktopLocation);
              } else if (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).contains(fileName)) {
                   retval = QStandardPaths::displayName(QStandardPaths::DocumentsLocation);
              } else if (QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).contains(fileName)) {
                   retval = QStandardPaths::displayName(QStandardPaths::DownloadLocation);
              } else if (QStandardPaths::standardLocations(QStandardPaths::FontsLocation).contains(fileName)) {
                   retval = QStandardPaths::displayName(QStandardPaths::FontsLocation);
              } else if (QStandardPaths::standardLocations(QStandardPaths::GenericCacheLocation).contains(fileName)) {
                   retval = QStandardPaths::displayName(QStandardPaths::GenericCacheLocation);
              } else if (QStandardPaths::standardLocations(QStandardPaths::GenericConfigLocation).contains(fileName)) {
                   retval = QStandardPaths::displayName(QStandardPaths::GenericConfigLocation);
              } else if (QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).contains(fileName)) {
                   retval = QStandardPaths::displayName(QStandardPaths::GenericDataLocation);
              }
           }

           if (retval.isEmpty()) {
               retval = fileInfo.fileName();
           }
           return retval;
        } else {
            return fileInfo.fileName();
        }
    }

    Q_INVOKABLE static QMimeType get_mime_type(QString fileName) {
        QMimeDatabase db;
        return db.mimeTypeForFile(fileName);
    }

    Q_INVOKABLE static bool icon_exists(QString iconId) {
        return (QFile(QString(":/qml/qadfilemanager/img/%0.png").arg(iconId)).exists() || !QIcon::fromTheme(iconId).isNull());
    }

    Q_INVOKABLE bool is_special_path(QString path) {
        bool retval = false;
        QFileInfo fileInfo(path);

            QString fileName = fileInfo.absoluteFilePath();

            if (QStandardPaths::standardLocations(QStandardPaths::HomeLocation).contains(fileName)) {
                 retval = true;
            } else if (QStandardPaths::standardLocations(QStandardPaths::RuntimeLocation).contains(fileName)) {
                retval = true;
            } else if (QStandardPaths::standardLocations(QStandardPaths::TempLocation).contains(fileName)) {
                retval =true;
            } else if (QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation).contains(fileName)) {
                retval = true;
            } else if (QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).contains(fileName)) {
                retval = true;
            } else if (QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).contains(fileName)) {
                retval = true;
            } else if (QStandardPaths::standardLocations(QStandardPaths::MusicLocation).contains(fileName)) {
                retval = true;
            } else if (QStandardPaths::standardLocations(QStandardPaths::CacheLocation).contains(fileName)) {
                retval = true;
           } else if (QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).contains(fileName)) {
                retval = true;
           } else if (QStandardPaths::standardLocations(QStandardPaths::DataLocation).contains(fileName)) {
                retval = true;
           } else if (QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).contains(fileName)) {
                retval = true;
           } else if (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).contains(fileName)) {
                retval = true;
           } else if (QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).contains(fileName)) {
                retval = true;
           } else if (QStandardPaths::standardLocations(QStandardPaths::FontsLocation).contains(fileName)) {
                retval = true;
           } else if (QStandardPaths::standardLocations(QStandardPaths::GenericCacheLocation).contains(fileName)) {
                retval = true;
           } else if (QStandardPaths::standardLocations(QStandardPaths::GenericConfigLocation).contains(fileName)) {
                retval = true;
           } else if (QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).contains(fileName)) {
                retval = true;
           }

        return retval;
    }

    Q_INVOKABLE static QString get_icon_name(QString fileName) {
        QFileInfo fileInfo(fileName);
        QMimeType mimeType = get_mime_type(fileName);
        QString retval = mimeType.iconName();
        if (!fileInfo.isDir()) {
            retval = mimeType.iconName();
            if (!icon_exists(retval)) {
                retval = mimeType.genericIconName();
                if (!icon_exists(retval)) {
                    retval = "empty";
                }
            }
        } else {
           if (fileInfo.isRoot()) {
               retval = "folder-root";
           } else {
               QString fileName = fileInfo.absoluteFilePath();

               if (QStandardPaths::standardLocations(QStandardPaths::HomeLocation).contains(fileName)) {
                    retval = "user-home";
               } else if (QStandardPaths::standardLocations(QStandardPaths::RuntimeLocation).contains(fileName)) {
                   retval = "folder-runtime";
               } else if (QStandardPaths::standardLocations(QStandardPaths::TempLocation).contains(fileName)) {
                   retval = "folder-temp";
               } else if (QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation).contains(fileName)) {
                   retval = "folder-applications";
               } else if (QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).contains(fileName)) {
                   retval = "folder-pictures";
               } else if (QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).contains(fileName)) {
                   retval = "folder-videos";
               } else if (QStandardPaths::standardLocations(QStandardPaths::MusicLocation).contains(fileName)) {
                   retval = "folder-music";
               } else if (QStandardPaths::standardLocations(QStandardPaths::CacheLocation).contains(fileName)) {
                   retval = "folder-cache";
              } else if (QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).contains(fileName)) {
                   retval = "folder-config";
              } else if (QStandardPaths::standardLocations(QStandardPaths::DataLocation).contains(fileName)) {
                   retval = "folder-data";
              } else if (QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).contains(fileName)) {
                   retval = "user-desktop";
              } else if (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).contains(fileName)) {
                   retval = "folder-documents";
              } else if (QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).contains(fileName)) {
                   retval = "folder-download";
              } else if (QStandardPaths::standardLocations(QStandardPaths::FontsLocation).contains(fileName)) {
                   retval = "folder-fonts";
              } else if (QStandardPaths::standardLocations(QStandardPaths::GenericCacheLocation).contains(fileName)) {
                   retval = "folder-generic-cache";
              } else if (QStandardPaths::standardLocations(QStandardPaths::GenericConfigLocation).contains(fileName)) {
                   retval = "folder-generic-config";
              } else if (QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).contains(fileName)) {
                   retval = "folder-generic-data";
              } else {
                   retval = "folder";
               }
           }
        }
        return retval;
    }

    // made public due to qLess sorting
    QDate get_modified_date() { return m_fileInfo.lastModified().date(); }

signals:
    void fileChanged();

protected:
    QString get_owner_group() { return m_fileInfo.group(); }
    QString get_owner_user() { return m_fileInfo.owner(); }
    bool is_hidden() { return m_fileInfo.isHidden(); }
    bool is_executable() { return m_fileInfo.isExecutable(); }
    bool is_readable() { return m_fileInfo.isReadable(); }
    bool is_writable() { return m_fileInfo.isWritable(); }
    bool is_file() { return m_fileInfo.isFile(); }
    QString get_mimeType() { return get_mime_type(m_fileInfo.absoluteFilePath()).name(); }
    bool is_directory() { return m_fileInfo.isDir(); }
    QString get_absolute_filepath() { return m_fileInfo.absoluteFilePath(); }
    QString get_fileName() { return m_fileInfo.fileName(); }
    bool is_symbolic_link() { return m_fileInfo.isSymLink(); }
    QString get_symbolicLinkTarget() { return m_fileInfo.symLinkTarget(); }
    bool is_special_path() { return is_special_path(m_fileInfo.filePath()); }
    QString get_filePath() { return m_fileInfo.absolutePath(); }
    QDateTime get_modified() { return m_fileInfo.lastModified(); }

    QStringList get_emblems() {
        QStringList retval;
        if (!is_readable()) retval << "unreadable";
        if (is_readable() && !is_writable()) retval << "readonly";
        if (is_symbolic_link()) retval << "symbolic-link";
        if (is_executable()) retval << "executable";
        ///if (is_hidden()) retval << "hidden";
        return retval;
    }

    QString get_displayName() {
        return get_displayName(m_fileInfo.absoluteFilePath());
    }
    QString get_fileType() { return m_fileInfo.completeSuffix(); }
    QIcon get_icon() { return  QIcon::fromTheme(m_mimeType); }
    qint64 get_size() { return (is_directory() ? m_dirSize : m_fileInfo.size()); }

    QString get_icon_name() {
        return get_icon_name(m_fileInfo.absoluteFilePath());
    }
    QFileInfo m_fileInfo;
    QString m_mimeType;
    qulonglong m_dirSize;


};

#endif // FILEINFO_H
