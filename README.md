regex parser

a simple toy regex parser.

to build and run
----------------

```sh
meson setup build --buildtype=release
meson compile -C build
./build/regex "pattern" "string"
```
