# Dev Log:

This document must be updated daily by EACH group member.

## Gabriel

### 2025-01-06 - This night doesn't count SEE BELOW
I did some work in class, copy-pasting into proposal.md. Then I started working on server pipes. Unfortunately, I couldn't do anything in the evening brcause I was at the hospital the whole time with my grandfather.

### 2025-01-07 - Makefile, server/client
I continued working on client/server stuff, making a lot of progress. It's pretty much useable for my project now. I also wrote my presumptive makefile.

### 2025-01-08 - Updated forking server
Since I hadn't fully finished lab 15 before, I did that 1-7 and 1-8. I then copied my most accurate forking server code into this final project, along with changing some of the stuff in pipe_networking.h for this project specifically.

### 2025-01-09 - Again, doesn't count
I wasn't at school today for the family reasons you know about. It also hindered me from doing work at home.

### 2025-1-10,11 - Game starts appropriately
The start of the game works. The communication ebtween parent, child, and server is the toughest part of the project, and it seems to be going alright. I can use most of this code for each of the n rounds. The other parts necessary are the changing of the words and the realignment of child-client.

### 2025-1-13 - Sentences Edited
I wrote a function that edits the sentence by the mode/difficulty amount of characters. I also wrote the client side of the code. Currently, it doesn't switch the client-child every time, which is the hardest part of the project. However, without that, the basic code works.

### 2025-1-14 - In between rounds works
I dealt with the toughest part, slightyl changing my strategy. Instead of redoing handshakes, I did a lot of parent-child pipe communication stuff, with unnamed pipes both ways. This is in a "rotating" branch. I made the randomness actually random. The core stuff is nearly done. Lastly, I laid the groundwork on an expansion.

### 2025-1-15 - Mixed bag on numRounds
I spent a lot of time writing and then deleting different versions of numRounds, where numRounds > numPlayers. Right now, the entire game is one round late and it's weird.

### 2025-1-16 - Can't play on temp?
In class I worked on numRounds again. The first of the two broken parts is fixed, but it still ends one round early weirdly. But in class and in the dojo I ran into an annoying problem. Due to either premission issues or pipe limitations, I couldn't play with somebody else who ssh'ed into temp.

### 2025-1-17,18,19 - Permission, numRounds fixed
It now works by sshing and using /tmp. Additionally, you can play any number of rounds. I could switch that into 2 different modes. I also got work done on difficulties.
