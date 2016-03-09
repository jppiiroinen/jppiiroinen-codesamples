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
#include "support.h"
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QFileInfo>

Support::Support(QObject *parent) :
    QObject(parent)
{
    connect(&m_fileSystemWatcher,SIGNAL(directoryChanged(QString)),this,SLOT(refreshFileList()));
    connect(&m_fileSystemWatcher,SIGNAL(directoryChanged(QString)),this,SIGNAL(directoryChanged(QString)));
    connect(&m_fileSystemWatcher,SIGNAL(fileChanged(QString)),this,SIGNAL(fileChanged(QString)));

    QStringList paths = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);

    if (paths.count() == 0) { return; }

    QString nextPath = QDir(paths.at(0)).absolutePath();

    m_bShowDirs = false;
    m_bCalculateTotal = false;
    m_bShowHidden = false;
    m_bShowFiles = true;
    m_bOrderAsc = true;
    m_bOrderDirsLast = false;
    m_bOrderDirsFirst = false;
    m_bOrderType = false;
    m_bTechnicalUi = false;
    m_bSorted = true;
    m_sortBy = "SortBy_Name";
    m_bGroupByDate = false;
    m_bGroupByDateLatest = true;
    m_currentPath = nextPath;
    m_bIsRoot = QDir::rootPath().compare(m_currentPath)==0;

    m_fileSystemWatcher.addPath(m_currentPath);
    emit currentPathChanged();
    refreshFileList();
}

void Support::mkdir(QString path) {
    if (path.isEmpty()) {
        qDebug() << "PATH was empty";
        return;
    }
    QStringList paths = m_fileSystemWatcher.directories();
    QDir(paths.at(0)).mkdir(path);
    refreshFileList();
}

void Support::cd(QString path) {
    if (path.isEmpty()) {
        qDebug() << "PATH was empty";
        return;
    }
    QStringList paths = m_fileSystemWatcher.directories();
    QString nextPath = QDir(QString("%0/%1").arg(paths.at(0)).arg(path)).absolutePath();

    if (nextPath.length() == 0) { return; }

    m_currentPath = nextPath;
    emit currentPathChanged();
    m_bIsRoot = QDir::rootPath().compare(m_currentPath)==0;
   // qDebug() << "IS ROOT" << m_bIsRoot << QDir::rootPath().compare(m_currentPath);

    m_fileSystemWatcher.removePaths(m_fileSystemWatcher.directories());
    if (m_fileSystemWatcher.addPath(nextPath)) {
        refreshFileList();
    } else {
        m_fileSystemWatcher.addPath(paths.at(0));
        refreshFileList();
    }
}

qint64 Support::get_dirsize(QFileInfo fileInfo) {
    qint64 retval = 0;
    if (fileInfo.isDir()) {
        QDir dir(fileInfo.absoluteFilePath());

        QDir::Filters filters = QDir::AllEntries | QDir::NoDot;
        if (m_bShowDirs) {
            filters |= QDir::Dirs;
        }
        if (m_bShowFiles) {
            filters |= QDir::Files;
        }
        if (m_bShowHidden) {
            filters |= QDir::Hidden;
        }
        dir.setFilter(filters);

        QDirIterator it(dir, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            if (!it.fileInfo().isDir()) {
                retval += it.fileInfo().size();
            }
            it.next();
        }
    }
    return retval;
}

void Support::refreshFileList() {
    QStringList paths = m_fileSystemWatcher.directories();

    if (paths.isEmpty()) { return; }

    set_loading(true);

    QDir dir(paths.at(0));
    QDir::Filters filters = QDir::NoDotAndDotDot;
    if (m_bShowDirs) {
        filters |= QDir::Dirs;
    }
    if (m_bShowFiles) {
        filters |= QDir::Files;
    }
    if (m_bShowHidden) {
        filters |= QDir::Hidden;
    }

    QDir::SortFlags sort;
    if (m_bSorted) {
        if (m_sortBy.compare("SortBy_Name")==0) {
            sort = QDir::Name;
        } else if (m_sortBy.compare("SortBy_Modified")==0) {
            sort = QDir::Time;
        }  else if (m_sortBy.compare("SortBy_Size")==0) {
            sort = QDir::Size;
        }

        if (m_bOrderType) {
            sort |= QDir::Type;
        }
        if (m_bOrderDirsFirst) {
            sort |= QDir::DirsFirst;
        } else if (m_bOrderDirsLast) {
            sort |= QDir::DirsLast;
        }
        if (!m_bOrderAsc) {
            sort |= QDir::Reversed;
        }
    } else {
        sort = QDir::Unsorted;
    }
    dir.setSorting(sort);


    QFileInfoList entries = dir.entryInfoList(filters);
    m_fileList.clear();
    m_dirList.clear();

    foreach(FileInfo* fileInfo, m_allList) {
        fileInfo->deleteLater();
    }

    m_allList.clear();
    m_totalSize = 0;

    foreach(QFileInfo fileInfo, entries) {
        qint64 dirSize = 0;

        if (m_bCalculateTotal) {
            if (fileInfo.isDir() && fileInfo.fileName().compare("..")!=0 && fileInfo.fileName().compare(".")!=0) {
               dirSize += get_dirsize(fileInfo) + fileInfo.size();
            } else if (fileInfo.isFile()) {
               m_totalSize += fileInfo.size();
            } else {
               dirSize += fileInfo.size();
            }
            m_totalSize += dirSize;
        }

        FileInfo* fileInfoObj = new FileInfo(fileInfo, dirSize);


        if (fileInfo.isFile()) {
            m_fileList.append(fileInfoObj);
        } else if (fileInfo.isDir()) {
            m_dirList.append(fileInfoObj);
        }
        m_allList.append(fileInfoObj);

    }

    if (m_bGroupByDate) {
        if (m_bGroupByDateLatest) {
            qSort(m_fileList.begin(),m_fileList.end(),lessThan_date_latest);
            qSort(m_dirList.begin(),m_dirList.end(),lessThan_date_latest);
            qSort(m_allList.begin(),m_allList.end(),lessThan_date_latest);
        } else {
            qSort(m_fileList.begin(),m_fileList.end(),lessThan_date_oldest);
            qSort(m_dirList.begin(),m_dirList.end(),lessThan_date_oldest);
            qSort(m_allList.begin(),m_allList.end(),lessThan_date_oldest);
        }
    }

    set_loading(false);
    emit filesChanged();
}


void Support::_fileListAppend(QQmlListProperty<FileInfo> *property, FileInfo *file) {

}

FileInfo* Support::_fileListAt(QQmlListProperty<FileInfo> *property, int index) {
    return static_cast< QList<FileInfo *> *>(property->data)->at(index);
}

int Support::_fileListSize(QQmlListProperty<FileInfo> *property)
{
    return static_cast< QList<FileInfo *> *>(property->data)->count();
}

void Support::_fileListClear(QQmlListProperty<FileInfo> *property)
{
    static_cast< QList<FileInfo *> *>(property->data)->clear();
}

QQmlListProperty<FileInfo> Support::getFileList() {
    return QQmlListProperty<FileInfo>(this,&m_fileList,&_fileListAppend,&_fileListSize,&_fileListAt,&_fileListClear);
}

QQmlListProperty<FileInfo> Support::getDirList() {
    return QQmlListProperty<FileInfo>(this,&m_dirList,&_fileListAppend,&_fileListSize,&_fileListAt,&_fileListClear);
}

QQmlListProperty<FileInfo> Support::getAllList() {
    return QQmlListProperty<FileInfo>(this,&m_allList,&_fileListAppend,&_fileListSize,&_fileListAt,&_fileListClear);
}
