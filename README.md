# Seha's C Packages

A collection of tools to make your life easier.

## Setup

Create a structure like this:
```
project/inc/your_includes.h
project/src/main.c
project/Makefile
```
The `inc` folder is optional as all packages can be dependencies.

### Dependencies

Put your dependencies inside the `Makefile` as download links.

Example:
```
DEPS := \
https://your.source/nec.c \
https://your.source/nec.h \
```

These dependencies will automatically be managed and you can include them anywhere in your code!

**Note:** After adding or removing dependencies, make sure you run `make clean` so that next time `make` gets called, everything gets downloaded correctly.

### Libraries

Create a library folder of your choice and link it inside the `Makefile`, or link any other available library:
```
LIBS := -Llibs -lm
```

