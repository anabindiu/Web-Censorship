# Web-
1. How to run the code:
- To run the code, you need to log into the ssh server then open two terminals. On one terminal 
we want to run the code using the command: gcc A1-441.c -o A1-441. After running the code,
we want to start connecting to the client through our proxy server. To do that, we need to go to 
Firefox (download if u don’t have it), then go to settings and then Manual proxy configuration. In 
the HTTP Proxy box, we want to add in the IP address of your current internet address, then in the port 
number we want to add any number between 1025-65535. After we do that, we want to go back to our first 
terminal on which we ran the code and type: ./A1-441 [port number] (ie: ./A1-441 1400). Before entering 
that into the terminal, we want to open a new terminal on which we want to type the 
command: telnet 136.159.5.25 [port number] (ie: telnet 136.159.5.25 1400). Now, we can go 
back to our first terminal and enter the command. Then go to the second terminal and enter the 
other command. (We want to do this as fast as possible as there is a 5 second timeout). 
- After the client and proxy server connect, we then need to reload the page on firefox we want 
to block. Then in the second terminal we can either type: BLOCK [word] or UNBLOCK. 
Unblocking wouldn’t do anything unless we’ve already blocked a word. Therefore, we can enter
BLOCK [word] (ie: BLOCK curling) and then reload the page we want to block. After doing that 
the page should be blocked and an error page should pop up. To unblock the page, just type: 
UNBLOCK. 
