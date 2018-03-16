QT -= gui

CONFIG += c++14 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/FlaaOscServer.cpp \
    src/logging/MyLogger.cpp \
    src/handler/FLOPingHandler.cpp \
    src/handler/FLOModuleRepositoryHandler.cpp \
    src/handler/FLOModuleInstancesHandler.cpp \
    src/model/FLOModuleInstancesModel.cpp

HEADERS += \
    src/FlaaOscServer.h \
    src/logging/MyLogger.h \
    src/spdlog/details/async_log_helper.h \
    src/spdlog/details/async_logger_impl.h \
    src/spdlog/details/file_helper.h \
    src/spdlog/details/log_msg.h \
    src/spdlog/details/logger_impl.h \
    src/spdlog/details/mpmc_bounded_q.h \
    src/spdlog/details/null_mutex.h \
    src/spdlog/details/os.h \
    src/spdlog/details/pattern_formatter_impl.h \
    src/spdlog/details/registry.h \
    src/spdlog/details/spdlog_impl.h \
    src/spdlog/fmt/bundled/format.h \
    src/spdlog/fmt/bundled/ostream.h \
    src/spdlog/fmt/bundled/posix.h \
    src/spdlog/fmt/bundled/printf.h \
    src/spdlog/fmt/bundled/time.h \
    src/spdlog/fmt/fmt.h \
    src/spdlog/fmt/ostr.h \
    src/spdlog/sinks/android_sink.h \
    src/spdlog/sinks/ansicolor_sink.h \
    src/spdlog/sinks/base_sink.h \
    src/spdlog/sinks/dist_sink.h \
    src/spdlog/sinks/file_sinks.h \
    src/spdlog/sinks/msvc_sink.h \
    src/spdlog/sinks/null_sink.h \
    src/spdlog/sinks/ostream_sink.h \
    src/spdlog/sinks/sink.h \
    src/spdlog/sinks/stdout_sinks.h \
    src/spdlog/sinks/syslog_sink.h \
    src/spdlog/sinks/wincolor_sink.h \
    src/spdlog/sinks/windebug_sink.h \
    src/spdlog/async_logger.h \
    src/spdlog/common.h \
    src/spdlog/formatter.h \
    src/spdlog/logger.h \
    src/spdlog/spdlog.h \
    src/spdlog/tweakme.h \
    src/handler/FLOPingHandler.h \
    src/handler/FLOModuleRepositoryHandler.h \
        src/handler/FLOModuleInstancesHandler.h \
    src/model/FLOModuleInstancesModel.h

DISTFILES += \
    .astylerc \
    src/spdlog/fmt/bundled/LICENSE.rst \
    src/flaaoscsdk/README.md \
    src/flaaoscsdk/flaaoscsdk.pri \
    src/flaaoscsdk/flaaoscsdk.pri

include( src/flaaoscsdk/flaaoscsdk.pri)

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-flaarlib-Desktop_ad6991-Debug/release/ -lflaarlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-flaarlib-Desktop_ad6991-Debug/debug/ -lflaarlib
else:unix: LIBS += -L$$PWD/../build-flaarlib-Desktop_ad6991-Debug/ -lflaarlib

INCLUDEPATH += $$PWD/../flaarlib/src
DEPENDPATH += $$PWD/../build-flaarlib-Desktop_ad6991-Debug
