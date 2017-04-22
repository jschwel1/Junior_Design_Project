CCARGS = -mmcu=## -DF_CPU=1000000UL -Os
FILES  =  shift_out.c drive_test.c rover.c four_ultrasonics.c
OUTPUT = proj

default: USART

test: $(addsuffix .c, $(OUTPUT))
	@echo "------------- MAKEFILE STUFF ------------------------"
	@echo "Compiling {" $(FILES) "} ==> " $(OUTPUT)
	@echo "_____________________________________________________"
	avr-gcc -g -Wall -mmcu=atmega328 -O1 $(FILES) -o $(OUTPUT)
winprog: test
		avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock $(OUTPUT) $(OUTPUT).hex
		avrdude -c buspirate -P/COM5 -p atmega328 -U flash:w:$(OUTPUT).hex
program: test
	avrdude -c buspirate -P /dev/buspirate -p atmega328
	avrdude -c buspirate -P /dev/buspirate -p atmega328 -U flash:w:$(OUTPUT)
clean:
	-rm $(OUTPUT)
	-rm $(OUTPUT).hex



# Bus Pirate -> AVR
# red (RESET) -> pin 1
# GND -> pin 8
# VCC -> pin 7
# orange (MOSI) -> pin 17
# brown (MISO) -> pin 18
# yellow (SCK) -> pin 19
