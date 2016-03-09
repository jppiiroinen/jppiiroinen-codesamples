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
#ifndef SUPPORT_H
#define SUPPORT_H

#include <QFileInfoList>
#include <QObject>
#include <QFileSystemWatcher>
#include "fileinfo.h"

class Support : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<FileInfo> fileModel READ getFileList NOTIFY filesChanged )
    Q_PROPERTY(QQmlListProperty<FileInfo> directoryModel READ getDirList NOTIFY filesChanged)
    Q_PROPERTY(QQmlListProperty<FileInfo> allModel READ getAllList NOTIFY filesChanged)

    Q_PROPERTY(QString currentPath READ getCurrentPath NOTIFY filesChanged)
    Q_PROPERTY(QString currentPathDisplayName READ getCurrentPathDisplayName NOTIFY filesChanged)
    Q_PROPERTY(qint64 totalSize READ getTotalSize NOTIFY filesChanged)
    Q_PROPERTY(bool show_dirs READ show_dirs WRITE set_show_dirs NOTIFY filesChanged)
    Q_PROPERTY(bool show_files READ show_files WRITE set_show_files NOTIFY filesChanged)
    Q_PROPERTY(bool calculate_total READ calculate_total WRITE set_calculate_total NOTIFY filesChanged)
    Q_PROPERTY(bool is_root READ is_root NOTIFY filesChanged)
    Q_PROPERTY(bool show_hidden READ show_hidden WRITE set_show_hidden NOTIFY filesChanged)
    Q_PROPERTY(bool order_asc READ order_asc WRITE set_order_asc NOTIFY filesChanged)
    Q_PROPERTY(bool group_by_date READ groupByDate WRITE set_groupByDate NOTIFY filesChanged)
    Q_PROPERTY(bool group_by_date_latest READ groupByDateLatest WRITE set_groupByDateLatest NOTIFY filesChanged)
    Q_PROPERTY(bool order_dirs_first READ order_dirs_first WRITE set_order_dirs_first NOTIFY filesChanged)
    Q_PROPERTY(bool order_dirs_last READ order_dirs_last WRITE set_order_dirs_last NOTIFY filesChanged)
    Q_PROPERTY(bool order_type READ order_type WRITE set_order_type NOTIFY filesChanged)
    Q_PROPERTY(bool sorted READ sorted WRITE set_sorted NOTIFY filesChanged)
    Q_PROPERTY(bool technical_ui READ technical_ui WRITE set_technical_ui NOTIFY filesChanged)
    Q_PROPERTY(QString sort_by READ get_sort_by WRITE set_sort_by NOTIFY filesChanged)

    Q_PROPERTY(bool is_loading READ is_loading NOTIFY loadingChanged)


public:
    explicit Support(QObject *parent = 0);

    static bool lessThan_date_oldest(FileInfo* p1, FileInfo* p2) {
        return p1->get_modified_date() < p2->get_modified_date();
    }

    static bool lessThan_date_latest(FileInfo* p1, FileInfo* p2) {
        return p1->get_modified_date() > p2->get_modified_date();
    }

    Q_INVOKABLE void cd(QString path);

    Q_INVOKABLE void update() { refreshFileList(); }

    Q_INVOKABLE void mkdir(QString path);

    Q_INVOKABLE QString formatSize(qint64 totalSize) {
        QString retsuffix = "b";
        qint64 retvalue = totalSize;
        if (retvalue/1024 > 1) {
            retsuffix = "k";
            retvalue = retvalue/1024;
            if (retvalue/1024 > 1) {
                retvalue = retvalue/1024;
                retsuffix = "M";
                if (retvalue/1024 > 1) {
                    retsuffix = "G";
                    retvalue = retvalue/1024;
                }
            }
        }

        return QString("%0%1").arg(retvalue).arg(retsuffix);
    }

signals:
    void filesChanged();
    void directoryChanged(QString path);
    void fileChanged(QString path);
    void currentPathChanged();
    void loadingChanged();

protected slots:

    void set_sorted(bool isSorted) {
        if (m_bSorted == isSorted) return;
        m_bSorted = isSorted;
        refreshFileList();
    }
    bool sorted() { return m_bSorted; }


    void set_groupByDate(bool groupByDate) {
        if (m_bGroupByDate == groupByDate) return;
        m_bGroupByDate = groupByDate;
        refreshFileList();
    }
    bool groupByDate() { return m_bGroupByDate; }

    void set_technical_ui(bool technical_ui) {
        if (m_bTechnicalUi == technical_ui) return;
        m_bTechnicalUi = technical_ui;
        refreshFileList();
    }
    bool technical_ui() { return m_bTechnicalUi; }


    void set_groupByDateLatest(bool groupByDateLatest) {
        if (m_bGroupByDateLatest == groupByDateLatest) return;
        m_bGroupByDateLatest = groupByDateLatest;
        refreshFileList();
    }
    bool groupByDateLatest() { return m_bGroupByDateLatest; }



    void set_order_dirs_first(bool orderDirsFirst) {
        if (m_bOrderDirsFirst == orderDirsFirst) return;
        m_bOrderDirsFirst = orderDirsFirst;
        if (m_bOrderDirsFirst) m_bOrderDirsLast = false;
        refreshFileList();
    }

    void set_order_dirs_last(bool orderDirsLast) {
        if (m_bOrderDirsLast == orderDirsLast) return;
        m_bOrderDirsLast = orderDirsLast;
        if (m_bOrderDirsLast) m_bOrderDirsFirst = false;
        refreshFileList();
    }

    void set_order_type(bool orderType) {
        if (m_bOrderType == orderType) return;
        m_bOrderType = orderType;
        refreshFileList();
    }

    void set_sort_by(QString column) {
        if (m_sortBy.compare(column)==0) return;
        m_sortBy = column;
        refreshFileList();
    }

    void set_order_asc(bool asc) {
        if (m_bOrderAsc == asc) { return; }
        m_bOrderAsc = asc;
        refreshFileList();
    }

    bool order_asc() { return m_bOrderAsc; }

    QString get_sort_by() {
        return m_sortBy;
    }

    void set_loading(bool loading) {
        if (m_bLoading == loading) return;
        m_bLoading = loading;
        emit loadingChanged();
    }

    qint64 get_dirsize(QFileInfo fileInfo);
    void refreshFileList();

    bool order_dirs_first() { return m_bOrderDirsFirst; }
    bool order_dirs_last() { return m_bOrderDirsLast; }
    bool order_type() { return m_bOrderType; }

    bool is_root() { return m_bIsRoot; }
    bool show_dirs() { return m_bShowDirs; }
    bool show_hidden() { return m_bShowHidden; }
    bool calculate_total() { return m_bCalculateTotal; }
    bool show_files() { return m_bShowFiles; }
    bool is_loading() { return m_bLoading; }

    void set_calculate_total(bool calculate_total) {
        if (m_bCalculateTotal == calculate_total) return;
        m_bCalculateTotal = calculate_total;
        refreshFileList();
    }

    void set_show_files(bool show_files) {
        if (m_bShowFiles == show_files) return;
        m_bShowFiles = show_files;
        refreshFileList();
    }

    void set_show_dirs(bool show_dirs) {
        if (m_bShowDirs == show_dirs) return;
        m_bShowDirs = show_dirs;
        refreshFileList();
    }

    void set_show_hidden(bool show_hidden) {
        if (m_bShowHidden == show_hidden) return;
        m_bShowHidden = show_hidden;
        refreshFileList();
    }

    QString getCurrentPathDisplayName() {
        return FileInfo::get_displayName(m_currentPath);
    }

    qint64 getTotalSize() { return m_totalSize; }
    QString getCurrentPath() { return m_currentPath; }
    QQmlListProperty<FileInfo> getFileList();
    QQmlListProperty<FileInfo> getDirList();
    QQmlListProperty<FileInfo> getAllList();
    static void _fileListAppend(QQmlListProperty<FileInfo> *property, FileInfo *file);
    static void _fileListClear(QQmlListProperty<FileInfo> *property);
    static int _fileListSize(QQmlListProperty<FileInfo> *property);
    static FileInfo* _fileListAt(QQmlListProperty<FileInfo> *property, int index);

protected:
    QFileSystemWatcher m_fileSystemWatcher;
    QList<FileInfo*> m_fileList;
    QList<FileInfo*> m_allList;
    QList<FileInfo*> m_dirList;
    QString m_currentPath;
    QString m_sortBy;
    qint64 m_totalSize;
    bool m_bIsRoot;
    bool m_bShowDirs;
    bool m_bCalculateTotal;
    bool m_bShowHidden;
    bool m_bShowFiles;
    bool m_bOrderAsc;
    bool m_bLoading;
    bool m_bSorted;
    bool m_bGroupByDateLatest;
    bool m_bGroupByDate;
    bool m_bOrderDirsFirst;
    bool m_bOrderDirsLast;
    bool m_bOrderType;
    bool m_bTechnicalUi;
};

#endif // SUPPORT_H
