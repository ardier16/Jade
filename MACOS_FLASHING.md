# macOS Jade Development Flashing

These notes cover flashing a Jade development device from macOS. They are for
development or DIY hardware only. Do not raw-flash official retail Jade units;
use Blockstream Green or `FWUPDATE.md` for retail firmware updates.

## One-time macOS setup

Install the basic build tools:

```sh
brew install cmake ninja gnu-sed
```

Clone and set up ESP-IDF. Jade currently pins ESP-IDF from `Dockerfile`; at the
time these notes were written that is `v5.5.4`.

```sh
export ESP_IDF_BRANCH=$(grep ESP_IDF_BRANCH Dockerfile | sed 's/.*=//g')

mkdir -p ~/esp
cd ~/esp
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
git checkout "$ESP_IDF_BRANCH"
git submodule sync --recursive
git submodule update --init --recursive --force

./install.sh --enable-gdbgui esp32 esp32s3
```

The optional `qemu-xtensa` install is not required for flashing physical Jade
hardware. If it fails because `libSDL2` is missing, either skip it or install
SDL2:

```sh
brew install sdl2
python3 ./tools/idf_tools.py install qemu-xtensa
```

## First flash from this checkout

Start a fresh shell, then load ESP-IDF and prepare the Jade checkout:

```sh
. ~/esp/esp-idf/export.sh
git submodule update --init --recursive
```

Make macOS use GNU `sed`, clear stale CMake/IDF environment variables, and
remove any old build/config state:

```sh
export PATH="/opt/homebrew/opt/gnu-sed/libexec/gnubin:/usr/local/opt/gnu-sed/libexec/gnubin:$PATH"
unset CMAKE_TOOLCHAIN_FILE
unset IDF_TARGET

rm -rf build sdkconfig sdkconfig.defaults managed_components
```

Find the connected device if needed:

```sh
ls /dev/cu.*
```

For the ESP32-S3 USB/JTAG Jade development device used here:

```sh
export JADESERIALPORT=/dev/cu.usbmodem1101
```

Configure, build, and flash Jade v2 development firmware.

If you do not need ESP USB Serial/JTAG debugging, use the normal TinyUSB CDC
Jade serial endpoint instead:

```sh
./tools/switch_to.sh jade_v2 --dev
idf.py all
idf.py -p "$JADESERIALPORT" flash
```

If flashing has sync or baud-rate issues, retry at a slower baud rate:

```sh
idf.py -p "$JADESERIALPORT" -b 115200 flash
```

## Flash after making code changes

For ordinary code changes, do not rerun `switch_to.sh` unless you are changing
the target device or build options. Keep the same shell environment, or reload it
in a new shell:

```sh
. ~/esp/esp-idf/export.sh
export PATH="/opt/homebrew/opt/gnu-sed/libexec/gnubin:/usr/local/opt/gnu-sed/libexec/gnubin:$PATH"
unset CMAKE_TOOLCHAIN_FILE
unset IDF_TARGET
idf.py all
idf.py flash
```

Use this heavier reset only when changing device target, changing
`sdkconfig.defaults`, or recovering from CMake/configuration errors:

```sh
rm -rf build sdkconfig sdkconfig.defaults managed_components
./tools/switch_to.sh jade_v2 --dev --jtag
idf.py all
idf.py flash
```
