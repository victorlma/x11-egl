CC=gcc
DFLAGS=-ggdb -Wall -Wextra --std=c99 --pedantic
RFLAGS=-O2
LIBS=`pkg-config --libs x11 egl`
SRCDIR=src
BUILDIR=build
BINPATH=$(BUILDIR)/x11-OpenGL
DOCSPATH=docs
HEADERS=$(wildcard $(SRCDIR)/*.h))

.PHONY: debug release docs clean $(HEADERS)

debug: $(BINPATH)-debug
	@gdb ./$<

release: $(BINPATH)-release $(HEADERS)
	@./$<

$(BINPATH)-debug: $(SRCDIR)/main.c $(HEADERS)
	mkdir -p $(BUILDIR)
	$(CC) $(DFLAGS) -o $(BINPATH)-debug $< $(LIBS)


$(BINPATH)-release: $(SRCDIR)/main.c
	mkdir -p $(BUILDIR)
	$(CC) $(RFLAGS) -o $(BINPATH)-release $< $(LIBS)

docs: $(DOCSPATH)/x11-egl.latex
	latexmk -xelatex -output-directory=$(DOCSPATH) $<

clean:
	rm -rf $(BUILDIR) $(DOCSPATH)/{*.aux,*.fdb_latexmk,*.fls,*.log,*.pdf,*.xdv}
