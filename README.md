# get-image

1. Change into `linux_out`.
2. Execute `./build`.
3. The binaries will be in `linux_out/install/bin/`.

You can change to `x86_64-w64-mingw32_out` to cross-compile for Windows.

# Usage

`./get_image <PATH_TO_FRAGMENT_SHADER>`

Useful flags:
* `--persist` - causes the shader to be rendered until a button is pressed
* `--output <OUTPUT_FILE>` - a png file will be produced at the given location with the contents of the rendered shader
* `--vertex <PATH_TO_VERTEX_SHADER>` - provide a custom vertex shader file, rather than using the trivial one provided
