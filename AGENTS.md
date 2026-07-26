# Agent Notes

## Flash Jade After Code Changes

After making firmware code changes, build and flash Jade with:

```sh
. ~/esp/esp-idf/export.sh
export PATH="/opt/homebrew/opt/gnu-sed/libexec/gnubin:/usr/local/opt/gnu-sed/libexec/gnubin:$PATH"
unset CMAKE_TOOLCHAIN_FILE
unset IDF_TARGET
idf.py all
idf.py flash
```
