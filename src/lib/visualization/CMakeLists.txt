remake_qt4_ui(visualization)
remake_qt4_moc(visualization)

remake_add_library(visualization LINK ${QT_QTGUI_LIBRARY} ${QT_QTOPENGL_LIBRARY}
  ${FTGL_LIBRARIES} base sensor)
remake_add_headers(INSTALL visualization)
