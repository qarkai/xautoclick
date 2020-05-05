# xAutoClick

## Note

It's the fork of [xAutoClick](http://xautoclick.sourceforge.net/).

## Description

This autoclicker application enables you to automatically click the left mousebutton. It is designed to reduce strain on the wrist muscles caused by repetitive mouse clicking, which can lead to serious injury or impair (RSI, Carpal Tunnel Syndrome, et cetera). By specifying the interval, a random factor and the number of times to click, the program simulates natural clicking.

Possible fields of use are (obviously) programs that require a lot of repetitive clicking, but it can also be used to click restart every ten minutes in a presentation program.

Currently, xAutoClick has GUI front-ends for X11/console, X11/GTK+ 3.0, X11/Qt 5.x and X11/FLTK and has been tested on Linux/x86.

## Compilation

Be sure you have the proper development packages for your distribution installed. For example, on Debian you need at least one of qtbase5-dev, libgtk-3-dev or libfltk1.3-dev. Also, be sure to have the libevdev and XTest extension and their development files. Again, on Debian, you need libevdev, libevdev-dev, libxtst6 and libxtst-dev.

After that, run:

```sh
mkdir build
cd build
cmake ..
```

and if all is fine (at least one GUI must be set to yes), run:

```sh
cmake --build .
sudo cmake --build . --target install
```

## Authors

Core, ASCII, GTK1, GTK2, QT3, QT4, FLTK, Command Line, build system: [Ivo van Poorten](https://github.com/ivop) <ivop@euronet.nl> <ivop@free.fr>

GTK3, Qt5, CMake build system, libevdev: [Arkadiy Illarionov](https://github.com/qarkai)
