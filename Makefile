include llvm-mos\Makefile.pref

CFLAGS += -Os
LDFLAGS += -Wl,--print-memory-usage
OBJS += context.o kernel.o exception.o entry.o #thread.o 
BIN = X16IDE.PRG

$(BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $(BIN) $(OBJS)

noelf: $(BIN)
	del $(BIN).elf

clean:
	del $(OBJS) $(BIN)

all: $(BIN)

run: $(BIN)
	$(EMU) -prg $(BIN) -zeroram -debug -run -ram 2048 -echo -scale 2