# Status
- Working so far:
- -> Main Hardware assembled 
- -> Audio playback possible (from SD-Card)

# IDE and branches git
- CLion and PlatformIO recommended
- Working on separate branches when writing code
- one branch for every subproject 

# PCB design
- Software: Keycut
- design with port extension of 
- -> Micro-USB extension to USB-C
- -> audiojack extension

# Hardware refinements/additions
- go back button
- play button

# Flowchart of software structure/ menu design
- loading of software samples
- different submenus depending on main action (left hand side vertical menu) [Load, Record, Manage, SNDS]
- selection of sample packs on SD-Card
- Load
- -> selection of sample packs
- Record
- -> 
- Manage
- -> Change mapping / change groups
- -> Sound Editor
- --> Change Pitch
- --> Reverse
- --> Volume
- --> FX
- Audio
- -> Record Midi
- -> Session Settings
- -> Resample [IMPORTANT]
- --> current loop
- -> FX
- --> Tracks
- ---> Apply FX
- Main Settings
- -> Save
- -> Edit
- SNDS

# improvement ideas
- buttons for switching between most important menu points / submenu points

# rough draft of software
- interrupt/input handler
- -> midi 
- -> audio

# TODO
- interrupt handler: Lucas
- -> handling different input signals
- window/UI handler: Dennis
- hardware test extension: David
- -> audio flash storage access
- pcb design: Alex
- -> additional LED?
- -> two buttons for back and apply/play
- -> probably 4 sensitive resistor pads
- hardware test, midi and flash test: David
- memory and SD-Card read/write, folders: Lucas, Lena
- assembling hardware sampler for uni: Lena

# next appointment
- with Mr Reiter, either
- -> Tuesday, 3rd May 1pm until 4pm
- -> Wednesday, 4th May until 5pm
- -> or Thursday, 5th May from 1pm
