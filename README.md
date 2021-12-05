# videogames

This repository contains a collections of videogames that I made many years ago.
I have multiple times forgotten how to build the games on Linux (I used Visual Studio on Windows at the time), so I am archiving that information on GitHub.

## Preparation

These instructions are for Linux systems.

The games all use the [SFML](https://www.sfml-dev.org/index.php) library.

On Ubuntu you can install the shared libraries and headers with:

```shell
sudo apt install libsfml-dev
```

On Arch Linux you can use:

```shell
sudo pacman -S sfml
```

Then make sure to clone the repository recursively:

```shell
git clone --recursive https://github.com/BarrensZeppelin/videogames
```

## Games

The games use two old libraries that I made.
Build them first by running the `build.sh` scripts in the `bzsf` and `TBC-1.0` folders.

Each game subfolder contains a script named `build.sh` that will produce a `game` executable when run.
