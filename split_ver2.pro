TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.c \
    split_ver2.c

linux: {
    message("UNIX")

    CONFIG(debug, debug|release) {
      message("DEBUG")
      DEFINES += _DEBUG
    } else:CONFIG(release, release|debug) {
        message("RELEASE")
    }
} else {
    message("NOT UNIX")
}

HEADERS += \
    split_ver2.h

