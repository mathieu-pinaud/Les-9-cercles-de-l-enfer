TEMPLATE = app
TARGET = project

CONFIG += console
CONFIG -= app_bundle

QT += widgets

SOURCES += \
	main.cpp \
	../window/window.cpp \
	../LPTF_Socket/LPTF_Socket.cpp \
	../LPTF_Socket/socket_Client/socket_Client.cpp \
	../LPTF_Socket/socket_Server/socket_Server.cpp

HEADERS += \
	../LPTF_Socket/LPTF_Socket.hpp \
	../window/window.hpp \
	../LPTF_Socket/socket_Client/socket_Client.hpp \
	../LPTF_Socket/socket_Server/socket_Server.hpp

LIBS += -lstdc++

QMAKE_CXXFLAGS += -g3 -Wall -Wextra -Werror