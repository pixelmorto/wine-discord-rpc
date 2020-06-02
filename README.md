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

Run the script with your `WINEPREFIX` set (`~/.wine` is an example):
```
WINEPREFIX=~/.wine setup-wine-discord-rpc install
```
This will create the required registry key and symlinks for Discord RPC to work.

For Proton prefixes, run the script with your `wine_path` set appropriately to a Proton installation and find a Proton game's AppID in your SteamLibrary compatdata folder. `steamapps/compatdata/appid`.
```
$ export wine_path=~/.steam/steam/steamapps/common/[YOUR_PROTON_HERE]/dist/bin
OR
$ export wine_path=~/.steam/steam/compatibilitytools.d/[YOUR_PROTON_HERE]/dist/bin

$ export WINEPREFIX=~/.steam/steam/steamapps/compatdata/[APPID_HERE]/pfx
$ setup-wine-discord-rpc install
```

## Improvements

Feel free to send pull requests and/or raise issues if things are not working properly or it could use improvement.

## TODO

- make a PKGBUILD
- better ways to update the rpc

## License
MIT License.
