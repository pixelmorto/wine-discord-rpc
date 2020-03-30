# wine-discord-rpc

## Introduction

This is a program made for Wine that redirects all buffer reads/writes from the Windows discord-ipc pipe to the UNIX discord-ipc socket.

This does not require any modification to any discord-rpc or discord-game-sdk libraries, meaning all games should work and be able to communicate with native Discord.

The program also makes itself a Wine system process, so it closes out automatically when there are no non-system processes running in the wineprefix.

## Compilation

Compile using winegcc.

## How to use

The program can be ran with a program like so:
```
wine wine-discord-rpc.exe.so & sleep 1 && wine app_name_here.exe
```

## Improvements

Feel free to send pull requests and/or raise issues if things are not working properly or it could use improvement.
