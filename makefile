FILES          = DiceInvaders.cpp Engine.cpp entities.cpp Game.cpp
EXECUTABLE     = DiceInvaders.exe
CFLAGS         = /nologo /Z7 /W4 /WX /Fe$(EXECUTABLE)
CFLAGS_RELEASE = $(CFLAGS) /Ox /MT /DNDEBUG
CFLAGS_DEBUG   = $(CFLAGS) /Od /MTd /D_DEBUG
CC             = cl.exe
DEVENV         = devenv.exe

debug: dummy
	$(CC) $(CFLAGS_DEBUG) $(FILES)

release: dummy
	$(CC) $(CFLAGS_RELEASE) $(FILES)

dev: debug
	$(DEVENV) $(EXECUTABLE)

clean: dummy
	-@del $(EXECUTABLE)
	-@del *.obj
	-@del *.pdb
	-@del *.ilk

dummy:


