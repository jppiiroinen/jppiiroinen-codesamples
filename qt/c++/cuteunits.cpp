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

#include "cuteunits.h"

#include <QApplication>
#include <QDesktopWidget>
#include <qmath.h>
#include <QScreen>

CuteUnits* CuteUnits::m_pInstance = NULL;

CuteUnits::CuteUnits(QObject *parent) :
    QObject(parent), m_bInitialized(false)
{
     m_designResolution = QRect(0,0,1366,768);

     initialize();
}

QObject* CuteUnits::singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(scriptEngine)
    return CuteUnits::instance(engine);
}

void CuteUnits::initialize() {
    QDesktopWidget* desktop = qApp->desktop();
    QSizeF displaySize = qApp->primaryScreen()->physicalSize();
    double mmToInch = 0.0393700787;
    double displayWidthInch = displaySize.width()*mmToInch;
    double displayHeightInch = displaySize.height()*mmToInch;

    m_desktopGeometry = desktop->screenGeometry();
    m_displayDiagonalSize = qSqrt(qRound(displayWidthInch*displayWidthInch) + qRound(displayHeightInch*displayHeightInch));
    m_displayDiagonalSize = QString::number(m_displayDiagonalSize,'g',2).toDouble();

    m_defaultGrid = 6;

#if !defined(Q_OS_IOS) && !defined(Q_OS_ANDROID)
    m_devicePixelRatio = ((double)m_desktopGeometry.width()) / ((double)m_designResolution.width());
#else
    m_devicePixelRatio = qApp->devicePixelRatio();
#endif

    updateGridUnit();
    m_bInitialized = true;
}

void CuteUnits::updateGridUnit() {
#if !defined(Q_OS_IOS) && !defined(Q_OS_ANDROID)
    // On desktop we need to calculate using the pure resolution
    setGridUnit(m_defaultGrid * m_devicePixelRatio);
#else
// On mobile devices we need to take care to check the size of the display as well.
#if defined(Q_OS_IOS)
    if (CuteDevice::deviceModelIdentifier().contains("iPad"))
            setGridUnit(m_desktopGeometry.width()/133);
    else setGridUnit(m_desktopGeometry.width()/80);
#elif defined(Q_OS_ANDROID)
    if (CuteDevice::deviceModelIdentifier().contains("AndroidTablet"))
       setGridUnit(m_desktopGeometry.width()/128);
    else
        setGridUnit(m_desktopGeometry.width()/80);
#endif
#endif
}

void CuteUnits::setGridUnit(double unit) {
    m_gridUnit = unit;
    if (m_gridUnit<=0) {
        m_gridUnit = m_defaultGrid;
    }

    updateFonts();
    emit gridUnitChanged();
}

CuteUnits* CuteUnits::instance(QQmlEngine *engine) {
    if (!m_pInstance) {
        m_pInstance = new CuteUnits(engine);
    }
    return m_pInstance;
}

double CuteUnits::guToPx(double units) {
    return units * m_gridUnit;
}

double CuteUnits::pxToGu(double px) {
    return px / m_gridUnit;
}

void CuteUnits::setDesignResolution(int width, int height) {
    m_designResolution = QRect(0,0,width,height);
    initialize();
}

void CuteUnits::updateFonts() {
    m_fonts[FONT_XXLARGE] = guToPx(5);
    m_fonts[FONT_XLARGE] = guToPx(4.7);
    m_fonts[FONT_LARGE] = guToPx(4);
    m_fonts[FONT_MEDIUM] = guToPx(3.5);
    m_fonts[FONT_NORMAL] = guToPx(2.5);
    m_fonts[FONT_SMALL] = guToPx(2);
    m_fonts[FONT_TINY] = guToPx(1.2);
}
