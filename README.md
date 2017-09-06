UnrealPianofall - An Unreal Engine 4 Pianofall Remake
=====================================================

UnrealPianofall is a highly improved Unreal Engine 4 open-source remake of the original closed-source Pianofall Unity application by Stepan Artemyev. UnrealPianofall was made and is highly optimised for black MIDI. It suports unlimited MIDI file sizes which don't even need to fit into RAM because they get pharsed as stream and so the size does no longer matter. However the more RAM you have the higher the block limit can be. With 32 GB of RAM 500'000 blocks should be possible. The original Pianofall version needs over 10 times more RAM per block!

## How to Download Unreal Pianofall
Windows x64: https://mega.nz/#F!Etc2iL5a!GLHgNh81azbTK_m-9SyoYg

## Why should I use UnrealPianofall
- Highly customizable without editing the Source
- Unlimited MIDI file size using fast direct stream reading without using any MIDI reading librarys
- Pianofall plays in the beautiful map of "A Boy and his Kite" instead of a nearly empty room
- Much faster by using less performance compared to the original Pianofall
- 45 materials and 3 different Blocktypes available over command line arguments
- Possible to change a lot using the Unreal Editor like Map and Object without even need to programm anything
- Possible to completely change everything with only very bery basic C++ programming knowledge
- Looks much better due to a modern and highly evolved game engine with amazing graphical effecs like Temporal AA
- Much more accurate physics using PhysX. UnrealPianofall even uses async sub-stepping for even more accuracy
- Rendering huge Black MIDIs on a non high-end PC
- DirectX 10, 11, 12 and Vulkan support!
- Predefined camara movement supported

### How to use
1. Render images using UnrealPianofall
2. Use http://en.cze.cz/Images-to-video to generate an wmv video
3. Use https://github.com/KaleidonKep99/Keppys-MIDI-Converter/releases to generate wav audio
4. Use https://www.ffmpeg.org/ (ffmpeg -i video.wmv -i audio.wav -codec copy -shortest output.wmv) to merge audio and video withouit any quality lose

Tips: To improve the audio rendered using Keppy's MIDI Converter I recommend http://www.audacityteam.org/download/ (Effect: Limiter -0.5 db) and then export it as floating-point wav file before merging audio and video. All images can also be edited before converting them to video using a Photoshop batch script and fast export it as png afterwards. The video file will be the same (same file hash) as by using the compressed but much slower png export.

## Command line arguments
Instead of a fancy GUI I decided to only use command line arguments so that you can easily put them into diffrent batch files to optimize your productivity a lot. To make UnrealPianofall as customizable as Possible without the need for you to edit the source code I added as much as possible to hopefully cover any of your needs. If you miss one, or one isn't working properly just open an issue about it.

Excample how to call UnrealPianofall from CMD/PowerShell/BatchFile: .\UnrealPianofall_KiteBuild.exe midi="C:/Users/nico/Documents/UnrealPianofall_KiteBuild/WreckingBall_1Mio.mid" --path="C:/Users/nico/Documents/Out/" --limit 250000 --reduction 8 --capture --repeat

--midi=\"filepath\" => the MIDI file (.mid) to open
--path=\"dirpath\" => Screenshot save directory"
--PPQ => Overwrites the MIDI speed (pulses/quarter)
--limit => The max amount of cubes in the scene
--reduction => Max amount of cubes per frame per note
--block => 0=Cube (default), 1=Piano key, 2=Apple
--block_x => Spawn X-position (default: 90600.0)
--block_y => Spawn y-position (default: 645000.0)
--block_z => Spawn Z-position (default: -110000.0f)
--blockscale => Block size multiplier (default: 1.0)"
--material => 0=default, 1=basic, 2-43=starter [A-Z]
--gravity => Sets the worls gravity (dafault: -980.0)
--spawndist_x => Spawn X-offset multip. (default: 100)
--spawndist_y => Spawn Y-offset multip. (default: 100)
--width => Width res. (use together with --height)
--height => Height res. (use together with --width)
--mode => 0=Fullscreen, 1=WFull, 2=Wind. 3=NumWind.
--audio => Enable MIDI out on the default MIDI channel
--off => Enable sending MIDI off events to MIDI out
--capture => Saves a screenshot of every frame
--speed => Camara speed multiplicator (default: 1)
--no-repeat => Don't repeat pre-defined camera movement
--free => Disable camera and let you control the view
--tp => Teleports the player to the blocks if --free
--fix => Locks the camera after capture starts
--no-loading-movie => Disable the loading Movie
--fps => Limit the fps/speed to an specified framerate
--vsync => Turn ON VSYNC (limits midi speed like --fps)
--low =>  Default instead of the ultra graphic settings
--wait => Seconds for world optimisation befor playing
default: 7 seconds for loading the whole scene
also useful in combination with --free in order to
bring the camera in position befor capture start
--help => Displays the help dialog box on launch

## How to build
1. Download UnrealEngine
2. Download the "A Boy and his Kite" Demo project
3. Open the "A Boy and his Kite" demo project and add "Starter Content" to it.
4. Patch the Sources using Visual Studia 2017 like written in ABoyandHisKite_patch.txt
5. Add the files mentioned in ABoyandHisKite_patch.txt using the lates ones from this repository.

## How to contribute
- Create Pull Requests
- Report bugs, tell me your ideas or ask questions using GitHub's issue system
- Please nevetr post any Engine- or Kite Demo source code because https://www.unrealengine.com/en-US/eula doesn't allow it but instead make patches which doesn't contain any such code like I did for ABoyandHisKite_patch.txt

Have fun!