COMPILER := gcc
COMPILERFLAGS := `pkg-config --cflags gtk+-3.0 glib-2.0 vte-2.91`
LINKERFLAGS := `pkg-config --libs gtk+-3.0 glib-2.0 vte-2.91`
SRC := $(shell find . -name '*.c')
OUT := ./mnta

all: $(OUT)

$(OUT): $(SRC)
	$(COMPILER) $(COMPILERFLAGS) -o $(OUT) $(SRC) $(LINKERFLAGS)

debug:
	$(COMPILER) $(COMPILERFLAGS) -g -o $(OUT) $(SRC) $(LINKERFLAGS)

clean:
	rm -f $(OUT)
