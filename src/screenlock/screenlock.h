/***************************************************************************
**
** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (c) 2012 Jolla Ltd.
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef SCREENLOCK_H
#define SCREENLOCK_H

#include <QObject>
#include "touchscreen/touchscreen.h"

#include <QDBusMessage>

class QTimer;

/*!
 * The screen lock business logic is responsible for showing and hiding
 * the screen lock window and the event eater window when necessary.
 */
class ScreenLock : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool touchBlocked READ touchBlocked NOTIFY touchBlockedChanged FINAL)

public:
    /*!
     * Constructs a new screen lock business logic.
     *
     * \param parent the parent QObject for the logic
     */
    explicit ScreenLock(TouchScreen *touchScreen, QObject *parent = 0);

    /*!
     * Destroys the screen lock business logic.
     */
    virtual ~ScreenLock();

    /*!
     * Shows the screen lock window or the event eater window.
     *
     * \param service DBus service to call when unlock is performed.
     * \param path DBus path to call when unlock is performed.
     * \param interface DBus interface to call when unlock is performed.
     * \param method DBus method to call when unlock is performed.
     * \param mode The ScreenLockBusinessLogicAdaptor::TkLockMode opcode.
     * \param silent Whether to show a notification or not (deprecated)
     * \param flicker Deprecated/not used
     */
    int tklock_open(const QString &service, const QString &path, const QString &interface, const QString &method, uint mode, bool silent, bool flicker);

    /*!
     * Hides the screen lock.
     *
     * \param silent Whether to show notifications or not (deprecated).
     */
    int tklock_close(bool silent);

    /*!
     * Retuns the screen lock state.
     *
     * \return \c true if the screen is locked, \c false otherwise
     */
    bool isScreenLocked() const;

    /*!
     * Retuns the device locking state.
     *
     * \return \c true if the device is locked, \c false otherwise
     */
    bool isDeviceLocked() const;
    void setDeviceIsLocked(bool locked);

    /*!
     * Returns the low power mode state.
     *
     * \return \c true if the low power mode is on, \c false otherwise
     */
    bool isLowPowerMode() const;

    /*!
     * Returns the blanking policy state
     *
     * \return The blanking policy, which is one of notification, alarm
     * call, linger, or default
     */
    QString blankingPolicy() const;

    /*!
     * Returns touch blocking state.
     *
     * \return \c true when touch is blocked, \c false otherwise
     */
    bool touchBlocked() const;

    TouchScreen::DisplayState displayState() const;

public slots:
    //! Shows the screen lock window and calls the MCE's lock function.
    void lockScreen(bool immediate = false);

    /*!
     * Register interaction expected -state
     *
     * The lockscreen implementation should set this to true when the
     * ui state is such that no specific user interaction is expected,
     * and false when exit from such state is made.
     *
     * As an concrete example: When display is woken up and lockscreen
     * is shown -> set to true. When user swipes the lockscreen away
     * or to lock code entry view -> set to false.
     *
     * Primary consumer of this state data is mce - which uses it as an
     * input for display blanking policy.
     */
    void setInteractionExpected(bool expected);

    //! Timer callback for broadcasting interaction expected -state
    void interactionExpectedBroadcast();

    //! Hides the screen lock window and calls the MCE's unlock callback function.
    void unlockScreen();

private slots:
    //! Shows or hides the screen lock window
    void setScreenLocked(bool value);

    //! Shows or hides the event eater window
    void setEventEaterEnabled(bool value);

    //! Shows the screen lock window in normal mode and hides the event eater window.
    void showScreenLock();

    //! Shows the screen lock window in low power mode and hides the event eater window.
    void showLowPowerMode();

    //! Sets the screen lock extensions into display off state
    void setDisplayOffMode();

    //! Hides the screen lock window.
    void hideScreenLock();

    //! Hides the event eater window and the screen lock window.
    void hideScreenLockAndEventEater();

    //! Shows the event eater window.
    void showEventEater();

    //! Hides the event eater window.
    void hideEventEater();

    //! Handles LPM events coming from mce.
    void handleLpmModeChange(const QString &state);

    //! Handles blanking policy change signals from mce
    void handleBlankingPolicyChange(const QString &policy);

signals:
    //! Emitted when the screen lock state changes
    void screenLockedChanged(bool locked);

    //! Emitted when the device is locked or unlocked
    void deviceIsLockedChanged(bool locked);

    //! Emitted when the low power mode state changes
    void lowPowerModeChanged();

    //! Emitted when the display blanking policy changes
    void blankingPolicyChanged(const QString &policy);

    //! Emitted when touch blocking changes. Touch is blocked when display is off.
    void touchBlockedChanged();

    /*! Emitted when interaction expected -state changes.
     *
     * Uses under_score naming convention to maintain consistency with
     * other things already present in the dbus interface.
     */
    void interaction_expected(bool expected);

private:
    enum TkLockReply {
        TkLockReplyFailed = 0,
        TkLockReplyOk
    };

    enum TkLockMode {
        TkLockModeNone,           // Deprecated
        TkLockModeEnable,         // Show the lock UI in lock mode
        TkLockModeHelp,           // Deprecated
        TkLockModeSelect,         // Deprecated
        TkLockModeOneInput,       // Turn the event eater on
        TkLockEnableVisual,       // Show unlock UI
        TkLockEnableLowPowerMode, // Show low power UI
        TkLockRealBlankMode       // Display completely off
    };

    enum TkLockStatus {
        TkLockUnlock = 1,
        TkLockRetry,
        TkLockTimeout,
        TkLockClosed
    };

    TouchScreen *m_touchScreen;

    //! The MCE callback method
    QDBusMessage m_callbackMethod;

    //! Whether the system is shutting down or not
    bool m_shuttingDown;

    //! Whether the lockscreen is visible or not
    bool m_lockscreenVisible;

    //! Whether device is locked
    bool m_deviceIsLocked;

    //! Whether lock screen can be skipped if device is not locked
    bool m_allowSkippingLockScreen;

    //! Whether the lockscreen should be shown as low power mode
    bool m_lowPowerMode;

    //! The current blanking policy obtained from mce
    QString m_mceBlankingPolicy;

    //! Timer object for delayed interaction expected -state broadcasting
    QTimer *m_interactionExpectedTimer;

    //! Current internally cached interaction expected -state
    bool m_interactionExpectedCurrent;

    //! Latest interaction expected -state broadcasted over dbus
    int m_interactionExpectedEmitted;

#ifdef UNIT_TEST
    friend class Ut_ScreenLock;
#endif
};

#endif
