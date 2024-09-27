CFLAGS= -w -std=c++20
DBUS_FLAGS=$(shell pkg-config --cflags --libs dbus-cxx-2.0) -L/usr/local/lib/libdbus-cxx.so.2
SRC= *.cpp */*.cpp

run: build
	bin/ssnd

build: $(SRC)
	g++ $(SRC) $(CFLAGS) $(DBUS_FLAGS) -ldbus-1 -o bin/ssnd