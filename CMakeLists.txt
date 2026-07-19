cmake_minimum_required(VERSION 3.19)
project(Slay_The_Spire LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Multimedia)

find_package(Qt6 6.5 REQUIRED COMPONENTS Sql)

qt_standard_project_setup()

qt_add_resources(RESOURCES resources.qrc)
qt_add_resources(RESOURCES backgrounds.qrc)

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
    relic.h relic.cpp relic.ui
    allrelics.h allrelics.cpp
    allenemies.h allenemies.cpp
    mapButton.h mapButton.cpp
    map.h map.cpp map.ui
    database.h database.cpp
    cardIDeas.h
    campfire.h campfire.cpp
    shop.h shop.cpp
    event.h event.cpp
    carddialogs.h carddialogs.cpp
    allevents.h allevents.cpp
    backgrounds.qrc
    reward.h reward.cpp
    sounds.qrc
    mainmenu.h mainmenu.cpp mainmenu.ui
    login_signup.h login_signup.cpp login_signup.ui
    combatvisuals.h combatvisuals.cpp
    encountermanager.h encountermanager.cpp
    gamemanager.h gamemanager.cpp
    piledialog.h piledialog.cpp piledialog.ui

    #
)

target_link_libraries(Slay_The_Spire PRIVATE Qt6::Sql)

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
