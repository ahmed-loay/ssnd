# SSND - Simple Stupid Notification Daemon
<div align="center">

A lightweight semi-complaint notification daemon according to [the freedesktop.org spec](https://specifications.freedesktop.org/notification-spec/latest/protocol.html), perfect for scripting needs.

</div>


## 💬 Motivation
Tired of wrestling with dbus-monitor and awk to listen for notifications? SSND offers a lightweight yet handy solution for handling notifications directly from your scripts. ;)

While developing Shudder-bar, I thought I could just read notifications off of existing daemons and simply hide their existing pop-ups, quickly finding out later that it's hard if not impossible to, thus SSND.