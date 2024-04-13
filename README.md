# Sorting

https://github.com/seajee/sorting/assets/140245488/e88923aa-d030-4027-90fb-781b2ae76ed1

Sorting algorithms visualization, in C with raylib.

# Building

To build the project, you can run the following with GNU Make.

```bash
make
```

## Windows

To build static executable for Windows you need to have a correct MSYS2
environment and the raylib library installed for MinGW-w64. Then, you can run
the following.

```bash
make windows
```

## Usage

The application has a custom, immediate mode GUI to simplify it's use. Still,
to resize the array, you would need to execute the application like the
following.

```bash
./sorting 200 # array of 200 integers
```
