# wine-discord-rpc

## Introduction

This is a program made for Wine that redirects all buffer reads/writes from the Windows discord-ipc pipe to the UNIX discord-ipc socket.

This does not require the need to modify any discord-rpc or discord-game-sdk libraries, meaning all games should work and be able to communicate with native Discord.

The program also makes itself a Wine system process, so it closes out automatically when there are no non-system processes running in the wineprefix.

## Compilation

Compile using winegcc or patch into a Wine source tree using discord.patch and compile.

To create the patch, use a Wine git source tree and rebase the patch.

## How to use

This is meant to be integrated into a Wine source tree with a patch, so it is built with wine and wineboot can launch the program.

However, the program can be ran before the game as an add-on like so:
```
wine discord.exe.so && wine game_exe_here.exe
```

## Improvements

Feel free to send pull requests and/or raise issues if things are not working properly or it could use improvement.

Do not modify discord.patch, just modify discord.c when sending PRs.
