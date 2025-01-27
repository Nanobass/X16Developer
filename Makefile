include llvm-mos\Makefile.pref

OBJS +=entry.o thread.o context.o kernel.o assert.o memory.o
BIN = X16IDE.PRG

$(BIN): $(OBJS)
	$(LD) -o $(BIN) $(OBJS)

noelf: $(BIN)
	del $(BIN).elf

clean:
	del $(OBJS) $(BIN)

all: $(BIN)

run: $(BIN)
	$(EMU) -prg $(BIN) -zeroram -debug -run -ram 2048 -echo -scale 2