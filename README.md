# wine-discord-rpc
This is a program made for Wine that redirects all buffer reads/writes from the Windows discord-ipc pipe to the UNIX discord-ipc socket.
Does not require the need to modify any discord-rpc or discord-game-sdk libraries, meaning all games should work and be able to communicate with native Discord.
