
all: qmk_firmware build program

qmk_firmware:
	git clone https://github.com/qmk/qmk_firmware.git
	cd qmk_firmware; git reset --hard 1230ecb73d331bb218482eacc67c5dfa190def54
	patch qmk_firmware/tmk_core/common/action.c da64/keymaps/default/qmk_firmware.patch

build:
	rm -Rf qmk_firmware/keyboards/da64
	cp -Rf da64 qmk_firmware/keyboards/
	cd qmk_firmware; make da64:default

program:
# pro micro
# note: you have to manually connect RESET and GND a few times before running this
	avrdude -v -patmega32u4 -cavr109 -P/dev/ttyACM0 -b57600 -D \
	        -Uflash:w:qmk_firmware/.build/da64_default.hex:i

clean:
	rm -Rf qmk_firmware/.build
