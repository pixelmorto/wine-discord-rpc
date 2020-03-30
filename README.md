# wine-discord-rpc

## Introduction

This is a program built for Wine that gives the ability for other Wine processes to communicate with your native Discord client.

## Compilation

Compile using winegcc

## How to use

This can be ran like so:
```
wine wine-discord-rpc.exe.so & sleep 1 && wine app_name_here.exe
```

## Improvements

Feel free to send pull requests and/or raise issues if things are not working properly or it could use improvement.

## TODO

- Create a patch for proton that launches this program in the background
- Create a shell script that launches the program in the background and then launch the user specified program
- Bigger buffer size (is it needed?)
- better ways to update the rpc

## License
MIT License.
