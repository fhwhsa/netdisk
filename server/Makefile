.PHONY: debug run clean

SOURCES := \
	idatabase/idatabase.cpp	\
	ifilefolder/ifilefolder.cpp \
	json/jsoncpp.cpp \
	msg/msgUnit.cpp \
	msg/msgParsing.cpp \
	mysql/mysqlConn.cpp \
	mysql/connectionPool.cpp \
	wrap/wrap.cpp \
	server.cpp \
	run.cpp

SLIB := event mysqlclient
FLAGS := -Wall -O2 -std=c++20

_SLIB := $(foreach it, $(SLIB), -l$(it))
BASE := g++ $(SOURCES) $(_SLIB) $(FLAGS)
run: $(SOURCES)
	$(BASE) -o run

debug:
	$(BASE) -g -o drun

clean:
	rm -f run drun