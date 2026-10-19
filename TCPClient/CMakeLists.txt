 cmake_minimum_required(VERSION 3.19)
project(TcpClient LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

qt_add_executable(TcpClient
    WIN32 MACOSX_BUNDLE
    main.cpp
    tcpclient.cpp
    tcpclient.h
    tcpclient.ui
    config.qrc
    protocol.h
    protocol.cpp
    openwidget.h openwidget.cpp openwidget.ui
    online.h online.cpp online.ui
    friendwidget.h friendwidget.cpp friendwidget.ui
    privatechat.h privatechat.cpp privatechat.ui
    directoryope.h directoryope.cpp directoryope.ui
    resource.qrc
    share.h share.cpp share.ui



)

target_link_libraries(TcpClient
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
)

install(TARGETS TcpClient
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET TcpClient
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
