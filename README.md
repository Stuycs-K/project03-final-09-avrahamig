[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Vh67aNdh)
# CREATIVE BRILLIANT PROJECT NAME

### CREATIVE BRILLIANT GROUP NAME

Gabriel Avrahami
       
### Project Description:

It’s a game of telephone, actually multiple games going on concurrently. In between each person the server muddles the sentence a bit, according to whatever difficulty you would like. You can also play with sentences already created by me haphazardly.

### Project Link:

https://drive.google.com/file/d/11jvRhLOCpujtIM7P_d5-gyQfWaDUXTMz/view?usp=drive_link 
  
### Instructions:

You can play this multiple ways. 
- You can do it on the normal path. Playing with yourself might be boring but if you typed in your password onto somebody else's computer, they could play with you.
- Alternatively, on /tmp you can play with people ssh'ed into their own accounts, as long as you all ssh into the same computer. If you are going to your own /tmp and running "cp -r project03-final-09-avrahamig .", also be sure to chmod the new folder you created in /tmp to 775.

When you, the master, run the project you will:
1. Type "make"/"make compile" (either works)
2. Type "make run"
3. Follow the prompts on-screen.
   - If you want to play more rounds than you have players, you can do that. There are 40 sentences I created (at 2:30 AM), so you wont even have to generate your own sentences for any round > numPlayers.
   - The number of rounds played should be a multiple of the number of players. Otherwise you'll have an unfinished game.

Everyone else (i.e. other than you) should, once you've entered answers to the 3 questions, type: 
<br/>./player

If you want to play as well, join on another terminal and do ./player.
