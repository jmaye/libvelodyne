remake_qt4_ui(visualization)
remake_qt4_moc(visualization)

remake_add_library(visualization LINK ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY}
  ${QT_QTOPENGL_LIBRARY} ${FTGL_LIBRARIES} sensor)
remake_add_headers(INSTALL visualization)
