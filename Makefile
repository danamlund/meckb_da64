
all: qmk_firmware build program

qmk_firmware:
	git clone https://github.com/qmk/qmk_firmware.git
	cd qmk_firmware; git reset --hard ccb15c2d2923903a925d253eec66fd4356ceea85
	cd qmk_firmware; git apply ../da64/keymaps/default/qmk_firmware.patch
	cd qmk_firmware; make git-submodule

build:
	rm -Rf qmk_firmware/keyboards/da64
	cp -Rf da64 qmk_firmware/keyboards/
	cd qmk_firmware; make da64:default

program:
# pro micro
# note: you have to manually connect RESET and GND a few times before running this
	avrdude -v -patmega32u4 -cavr109 -P/dev/ttyACM1 -b57600 -D \
	        -Uflash:w:qmk_firmware/.build/da64_default.hex:i

clean:
	rm -Rf qmk_firmware/.build
