cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
mainwindow.cpp
make install DESTDIR=AppDir
AppDir/usr/local/bin/AR488-XFR-GUI
./appimage-builder-1.1.0-x86_64.AppImage --generate
./appimage-builder-1.1.0-x86_64.AppImage --recipe AppImageBuilder.yml
