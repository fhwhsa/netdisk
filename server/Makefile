.PHONY: debug run clean

SOURCES := \
	json/jsoncpp.cpp \
	msg/msgUnit.cpp \
	msg/msgParsing.cpp \
	mysql/mysqlConn.cpp \
	mysql/connectionPool.cpp \
	wrap/wrap.cpp \
	server.cpp

SLIB := event mysqlclient
FLAGS := -Wall -O2 -std=c++20

_SLIB := $(foreach it, $(SLIB), -l$(it))
BASE := g++ $(SOURCES) $(_SLIB) $(FLAGS)
run: $(SOURCES)
	$(BASE) -o server

debug:
	$(BASE) -g -o dserver

clean:
	rm -f server dserver