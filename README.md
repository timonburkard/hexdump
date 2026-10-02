# 🔎 hexdump

A small command-line hex dumper written in C. This project uses the [CLIC](https://github.com/timonburkard/clic) command line parser.

## Build

From the project root:

```sh
./build.sh
```

This produces the executable at the workspace root as `hexdump` resp. `hexdump.exe`.

## Run

```sh
$ ./hexdump show <input_file>
```

### Options

<pre>
$ ./hexdump show --help
Print hex dump of a file

<u>Usage:</u> hexdump show &lt;FILE&gt; [OPTIONS]

<u>Arguments:</u>
  &lt;FILE&gt;  File to hex dump or '-' for stdin

<u>Options:</u>
      --offset &lt;BYTES&gt;  Byte offset to start reading from
      --length &lt;BYTES&gt;  Maximum number of bytes to print
      --width  &lt;BYTES&gt;  Number of bytes per line
  -h, --help            Print help

</pre>

## Example

```sh
$ ./hexdump show file.bin
00000000  4D 5A 90 00 03 00 00 00 04 00 00 00 FF FF 00 00  |MZ..............|
00000010  B8 00 00 00 00 00 00 00 40 00 00 00 00 00 00 00  |........@.......|
00000020  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  |................|
00000030  00 00 00 00 00 00 00 00 00 00 00 00 80 00 00 00  |................|
00000040  0E 1F BA 0E 00 B4 09 CD 21 B8 01 4C CD 21 54 68  |........!..L.!Th|
00000050  69 73 20 70 72 6F 67 72 61 6D 20 63 61 6E 6E 6F  |is program canno|
00000060  74 20 62 65 20 72 75 6E 20 69 6E 20 44 4F 53 20  |t be run in DOS |
00000070  6D 6F 64 65 2E 0D 0D 0A 24 00 00 00 00 00 00 00  |mode....$.......|
...
00011440  5F 73 74 64 69 6F 5F 63 6F 6D 6D 6F 6E 5F 76 66  |_stdio_common_vf|
00011450  70 72 69 6E 74 66 00 5F 5F 69 6D 70 5F 64 61 79  |printf.__imp_day|
00011460  6C 69 67 68 74 00 5F 5F 70 5F 5F 5F 77 61 72 67  |light.__p___warg|
00011470  76 00 5F 5F 6D 69 6E 67 77 5F 61 70 70 5F 74 79  |v.__mingw_app_ty|
00011480  70 65 00                                         |pe.|
```
