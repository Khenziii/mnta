COMPILER := gcc
COMPILERFLAGS := `pkg-config --cflags gtk+-3.0 glib-2.0 vte-2.91 libcjson`
LINKERFLAGS := `pkg-config --libs gtk+-3.0 glib-2.0 vte-2.91 libcjson`
SRC := $(shell find . -name '*.c')
OUT := ./mnta

all: $(OUT)

$(OUT): $(SRC)
	$(COMPILER) $(COMPILERFLAGS) -o $(OUT) $(SRC) $(LINKERFLAGS) -lm

debug:
	$(COMPILER) $(COMPILERFLAGS) -g -o $(OUT) $(SRC) $(LINKERFLAGS) -lm

clean:
	rm -f $(OUT)
