/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef UT_LAUNCHERBUTTON_H
#define UT_LAUNCHERBUTTON_H

#include <QObject>

class MApplication;
class LauncherButton;

class Ut_LauncherButton : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test cases
    void testInitialization();
    void testInitializationXMaemoService();
    void testInitializationAbsoluteIcon();
    void testInitializationFreeDesktopIcon();
    void testLaunchApplication();
    void testLaunchMApplication();

signals:
    void clicked();

private:
    // MApplication
    MApplication *app;
    // The object being tested
    LauncherButton *m_subject;
};

#endif
