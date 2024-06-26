# Configuration du projet
TARGET = Dante

# Chemins des sources pour le client et le serveur
CLIENT_SOURCES = client_src/main.cpp \
                 LPTF_Socket/LPTF_Socket.cpp

SERVER_SOURCES = server_src/main.cpp \
                 LPTF_Socket/LPTF_Socket.cpp

# Compiler avec g++
QMAKE_CXX = g++

# Options de compilation
QMAKE_CXXFLAGS += -g3 -Wall -Wextra -Werror

# Configuration spécifique à Qt
QT += core

# Instructions pour la construction
CONFIG += console
CONFIG -= app_bundle

# Cibles supplémentaires pour la construction du client
client.target = $(TARGET)_client
client.commands = $$QMAKE_CXX $$QMAKE_CXXFLAGS $$CLIENT_SOURCES -o $$client.target
client.depends = $$CLIENT_SOURCES

# Cibles supplémentaires pour la construction du serveur
server.target = $(TARGET)_server
server.commands = $$QMAKE_CXX $$QMAKE_CXXFLAGS $$SERVER_SOURCES -o $$server.target
server.depends = $$SERVER_SOURCES

QMAKE_EXTRA_TARGETS += client server
