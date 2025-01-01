.PHONY: debug run clean

SOURCES := \
	buffer/buffer.cpp \
	connResources/connResources.cpp \
	idatabase/idatabase.cpp	\
	ifilefolder/ifilefolder.cpp \
	json/jsoncpp.cpp \
	log/log.cpp \
	msg/msgUnit.cpp \
	msg/msgParsing.cpp \
	mysql/mysqlConn.cpp \
	mysql/connectionPool.cpp \
	server/server.cpp \
	run.cpp

SLIB := event mysqlclient
FLAGS := -Wall -O2 -std=c++20

_SLIB := $(foreach it, $(SLIB), -l$(it))
BASE := g++ $(SOURCES) $(_SLIB) $(FLAGS)
debug:
	$(BASE) -g -o drun

run: $(SOURCES)
	$(BASE) -o run

clean:
	rm -f run drun