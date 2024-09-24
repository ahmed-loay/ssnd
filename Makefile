CFLAGS= -w -std=c++17
DBUS_FLAGS=$(shell pkg-config --cflags --libs dbus-cxx-2.0) -L/usr/local/lib/libdbus-cxx.so.2
SRC= main.cpp serverinfo_signature.cpp

run: build
	bin/noti-server

build: $(SRC)
	g++ $(SRC) $(CFLAGS) $(DBUS_FLAGS) -ldbus-1 -o bin/noti-server