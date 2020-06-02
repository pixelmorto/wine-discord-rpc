# wine-discord-rpc

## Introduction

This is a program built for Wine that gives the ability for other Wine processes to communicate with your native Discord client. This works with programs that are either dynamically or statically linked against discord-rpc or discord-game-sdk libraries.

## Compilation

Compile by doing:
```
make
```

To install system-wide:
```
sudo make install
```

To uninstall system-wide:
```
sudo make uninstall
```

To install for a package (on Arch Linux as an example):
```
DESTDIR=${pkgdir} make install
```

There needs to be an existing Wine installation with winegcc and multilib support!

## How to use

Run this script:
```
WINEPREFIX=~/.wine setup-wine-discord-rpc install
```
This will create the required registry key and symlinks for Discord RPC to work.

## Improvements

Feel free to send pull requests and/or raise issues if things are not working properly or it could use improvement.

## TODO

- Create a patch for proton that does the how-to automatically
- make a PKGBUILD
- better ways to update the rpc

## License
MIT License.
