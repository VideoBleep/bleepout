
Bleepout - Game Sequence & Logistics
==============


People wait in line

then N people enter the pen from the line
outside operator gives them wifi network information
they open browser
[technical: page loads causes them to enter lobby]

previous game ends and people in dome leave
outside operator sends all people who have successfully connected (presumably N people) into the dome
[technical: entire lobby (presumably N people) is transferred into the round]

inside operator asks "does everybody have the screen up on their phone?"
if yes, operator presses a button to send "Round Queue Event"
[technical: event is broadcast to phones, they enter calibration mode]

all the players run through calibration - asked "point at your paddle"
[technical: store offsets]

inside operator asks "everyone calibrated?"
if yes, operator presses a button "round start event"

dome shows "video bleep presents bleepout, etc."
N players get M stages to play (M=3, probably)

[technical: users' phones send controller data, balls bounce, etc.]

after last stage ends
show some scoring information on dome

inside operator asks players to leave
phone says "thanks for playing"
[technical: we give a cookie that expires in 3 minutes to prevent immediate rejoin]
signals outside operator to send in a new round

[technical: round is torn down, active players deleted]

Failure Modes
=============

Problem: Someone has trouble connecting to the network
Result: They hang out in pen until they figure it out, outside operator is there to assist 

Problem: Someone from the line (not in the pen) connects to the network and joins the lobby
Result: They aren't invited into dome, can't calibrate, and spend the game as a zombie. Can reload to try again after quit cookie expires

Problem: Player's phone does not have orientation support
Result: They will get onscreen controls 

Problem: Player does not have a smartphone
Result: We might have one or two onhand to loan out

Problem: Player wants to play again
Result: Cool! Get back in line, refresh page when you're in pen. Cookie will prevent immediate/abusive rejoins

Problem: Player does not want to leave the dome
Result: They game will have kicked their phone; they can watch for a bit, whatever

Problem: Player REALLY does not want to leave the dome
Result: Brute squad


