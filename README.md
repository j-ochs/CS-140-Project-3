# CS-140-Project-3

Here we can put ideas, links, and any other design outlines to the app.

RULES for iCu:

  This software will implement simultaneous client/server that implements a game of hide and seek called 'iCu' .

  The goal is to report as many iCu servers as you can.  

  Rules:  Whenever your laptop is on, it must be listening to TCP port 28900 .

  If a connection is made, and the client connecting issues the command "Who are you?\n" ,  your server must respond with your userid and the name of the access point or switch to which you are currently directly attached, separated by a space, and terminated with a newline, e.g. "rodkey vl-1a-wap3\n" .  

  No more than one query need be answered per 15 minute period. Superfluous connections must be disconnected without sending any data.

  The client portion of your software will scan for computers with port 28900 open, and connect as above.  Upon successfully encountering and querying a server, it will open a connection on port 28900 on pilot.westmont.edu and using HTTP GET syntax, browse to the following URL:

  http://pilot.westmont.edu:28900?i=<your (Links to an external site.) userID>&u=<their userID>&where=<their connection> 

  e.g. 

  http://pilot.westmont.edu:28900?i=rodkey&u=rwilder&where=vl-1a-wap3 (Links to an external site.)

  Use select to make sure that both the server and client can run independently.
