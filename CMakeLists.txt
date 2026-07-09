cmake_minimum_required(VERSION 3.19)
project(Slay_The_Spire LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Multimedia)

qt_standard_project_setup()

qt_add_resources(RESOURCES resources.qrc)

qt_add_executable(Slay_The_Spire
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    card.h card.cpp
    skillcards.h skillcards.cpp
    statuscards.h statuscards.cpp
    cursecards.h cursecards.cpp
    attackcards.h attackcards.cpp
    powercards.h powercards.cpp
    player.h player.cpp
    gameplay.h gameplay.cpp gameplay.ui
    enemy.h enemy.cpp
    allenemies.h allenemies.cpp
    relic.h relic.cpp
    allrelics.h allrelics.cpp

    ${RESOURCES}
    combatant.h combatant.cpp
    buffdebuff.h buffdebuff.cpp
    potion.h potion.cpp potion.ui
    topbar.h topbar.cpp topbar.ui
    relic.h relic.cpp
    allrelics.h allrelics.cpp
    allenemies.h allenemies.cpp
    #
)

target_link_libraries(Slay_The_Spire
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Multimedia
)

install(TARGETS Slay_The_Spire
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Slay_The_Spire
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
