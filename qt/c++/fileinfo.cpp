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
#include "fileinfo.h"

FileInfo::FileInfo(QFileInfo fileInfo, qulonglong dirSize, QObject *parent) :
    QObject(parent)
{
    m_fileInfo = fileInfo;
    m_dirSize = dirSize;

}

FileInfo::FileInfo(QObject* parent) : QObject(parent) {

}
