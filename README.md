# MultiplayerVideogame
<br>

## General Description
Simple game demo for the Windows console programmed in C++. The main objective was to build a client-server architecture, managing task separation between both sides, such as determining where the game logic should reside. The connection between the client and server is established using UDP messages parsed with JSON. To optimize server performance, threads are used to handle multiple clients simultaneously and independently.
