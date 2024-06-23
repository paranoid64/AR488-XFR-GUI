add_executable(AR488-XFR-GUI main.cpp mainwindow.cpp ressourcen.qrc)
target_link_libraries(AR488-XFR-GUI Qt5::Core Qt5::Widgets Qt5::Qml Qt5::Charts Qt5::SerialPort ${ZLIB_LIBRARIES})

install(TARGETS AR488-XFR-GUI DESTINATION bin COMPONENT binaries)
