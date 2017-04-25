CCARGS = -mmcu=## -DF_CPU=1000000UL -Os
FILES = test.c rover.c sonics_driver.c
FILE = test

default: test

test: $(addsuffix .c, $(FILE))
	avr-gcc -g -Wall -mmcu=atmega328 -O1 $(FILES) -o $(FILE)

program: test
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock test test.hex
	avrdude -c buspirate -P /dev/buspirate -p atmega328 -U flash:w:test
winprog: test
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock $(FILE) $(FILE).hex
	avrdude -c buspirate -P/COM5 -p atmega328 -U flash:w:$(FILE).hex

clean:
	-rm test.hex test
