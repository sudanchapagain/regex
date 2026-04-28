# REGEX

A basic and toy like regular expression parsing implementation.

### To Build and Run

```sh
meson setup build --buildtype=release
meson compile -C build
./build/regex "pattern" "string"
```

### credits

- <https://sh4dy.com/2025/05/01/regex_engine>: `src/main.cpp` follows this post one to one (i looked at their code and wrote mine. so its 1:1 but also not). So, all credit goes to sh4dy.
