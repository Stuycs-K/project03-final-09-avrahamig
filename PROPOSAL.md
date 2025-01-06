# Final Project Proposal

## Group Members:

Gabriel Avrahami
       
# Intentions:

It’s a game of telephone, actually multiple games going on concurrently. In between each person the server muddles the sentence a bit. The sentence can even be a terminal command, so at the end the computer can try to run it and see what horrors ensue.
    
# Intended usage:

When you, the master, run the project you include a number n as an argument. n will be how many people there are playing / how many games are occurring. The way you play is similar to what we did with the semaphore game. It will be on some public temp folder. People will connect to a main server to create their own sentence and edit the n-1 others.
  
# Technical Details:

I am responsible for all of the project. I will be using quite a few things we discussed in class. An int of shared memory will say how many characters were edited in between each round. There will be a forking server with n children (each child represents a game). Pipes will be used to communicate 1:1 between the n clients and n subservers. Each game’s output will be written to a file so you can see the full story. Lastly, each subserver will execvp its final command to see what happens.
    
# Intended pacing:

1/7 — Write skeleton files, finish lab 15 :)

1/9 — Use lab 15 to have the main and forked servers 

1/13 — Have the server be able to handle a transition round (taking in a sentence from a client, changing it slightly, sending it to the next subserver, and sending the new sentence to a client)

1/14 — Make the first round separate, with each client generating a new command

1/17 — Finish everything crucial to the project: shared memory, execvp, writing output

1/21 (due date) — Include a few nice-to-haves, like multiple difficulty modes. Maybe hard mode changes more letters. Maybe for easy mode you can’t change spaces, so people at least know how long each word is. Make the video.
