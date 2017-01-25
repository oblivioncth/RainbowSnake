QT += widgets
QT += multimedia

HEADERS += \
	snakeblock.h \
    keypresseventfilter.h \
    snake.h \
    snakewall.h
SOURCES += \
	main.cpp \
        snakeblock.cpp \
    keypresseventfilter.cpp \
    snake.cpp \
    snakewall.cpp

RESOURCES += \
    RSnake.qrc

RC_FILE = icon.rc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/collidingmice
INSTALLS += target

FORMS +=
