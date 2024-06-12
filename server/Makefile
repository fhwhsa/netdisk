.PHONY: debug run clean
HEADERSPATH := headers headers/inc_libevent headers/inc_mysql
SOURCES := sources/dataUnit.cpp sources/server.cpp sources/wrap.cpp sources/mysqlConn.cpp
SLIB := event mysqlclient
SLIBPATH := lib
FLAGS := -Wall -O2 -std=c++20


_HEADERSPATH := $(foreach it, $(HEADERSPATH), -I$(it))
_SLIB := $(foreach it, $(SLIB), -l$(it))
_SLIBPATH := $(foreach it, $(SLIBPATH), -L$(it))
BASE := g++ $(SOURCES) $(_HEADERSPATH) $(_SLIB) $(_SLIBPATH) $(FLAGS)
run: $(SOURCES)
	$(BASE) -o server

debug:
	$(BASE) -g -o dserver

clean:
	rm -f server dserver