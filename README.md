# c-sock-client

TCP / IPV4 Socket Client - Written in C

## Lifecycle
* Client connects to socket server via TCP and sends welcome message
* Client waits for data from server
* Client receives data (bytes converted to string, usually a command prompt command), parses it, and if a special command, 
run the function the special command is bound to. If NOT a special command, run it in an instance of command prompt via _popen

## Known Issues
* Error handling is almost non existent. Attmepted implenation, but I cannot properly convert the recv response to the
command string sent by server to a char to check for value. Further study into recv response is required.
