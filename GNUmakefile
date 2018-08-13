name := 9Be8Be
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/architecture.gmk

#CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)
#EXTRALIBS += $(shell $(ROOTSYS)/bin/root-config --libs)
#LDLIBS += $(shell $(ROOTSYS)/bin/root-config --libs)

CPPFLAGS += `root-config --cflags`
EXTRALIBS += `root-config --libs`
LDFLAGS += `root-config --libs`

include $(G4INSTALL)/config/binmake.gmk
