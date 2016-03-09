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
#include "cutesharedmemory.h"

bool CuteSharedMemory::write(QVariant data) {
    bool retval = false;
    if (m_sharedMemory.isAttached()) m_sharedMemory.detach();

    QBuffer buffer;
    if (buffer.open(QBuffer::ReadWrite)) {
        QDataStream out(&buffer);
        out << data;
        qint64 size = buffer.size();
        if (m_sharedMemory.create(size)) {
            m_sharedMemory.lock();
            char* sharedData = (char*)m_sharedMemory.data();
            const char* bufferData = buffer.data().data();
            memcpy(sharedData,bufferData,qMin(m_sharedMemory.size(),(int)size));
            m_sharedMemory.unlock();
            retval = true;
        }
    }
    return retval;
}

QVariant CuteSharedMemory::read() {
    QVariant retval;

    if (m_sharedMemory.attach()) {
        QBuffer buffer;
        QDataStream in(&buffer);
        m_sharedMemory.lock();
        buffer.setData((const char*)m_sharedMemory.constData(),m_sharedMemory.size());
        if (buffer.open(QBuffer::ReadOnly)) {
            in >> retval;
        }
        m_sharedMemory.unlock();
        m_sharedMemory.detach();
    }
    return retval;
}
