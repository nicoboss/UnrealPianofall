UnrealPianofallLight - An Unreal Engine 4 Pianofall Remake
==========================================================

UnrealPianofallLight is a highly improved Unreal Engine 4 open-source remake of the original closed-source Pianofall Unity application by Stepan Artemyev. UnrealPianofalLight was made and is highly optimised for black MIDI. It suports unlimited MIDI file sizes which don't even need to fit into RAM because they get pharsed as stream and so the size does no longer matter. However the more RAM you have the higher the block limit can be. With 32 GB of RAM 500'000 blocks should be possible. The original Pianofall version needs over 10 times more RAM per block!

## How to Download Unreal Pianofall Light
See https://github.com/nicoboss/UnrealPianofall/releases

## Why should I use UnrealPianofallLight
- Uses less lnternet bandwith (less then 100 MB), RAM, CPU and GPU compared to UnrealPianofall (Kite build)
- Support high-Resolution screenshots so that even owners of a Full HD screen will be able to render e.g. 8K footage
- Highly customizable without any programming knowledge. Download, open with UE4 Editor and modifying can start!
- Just open the sources it with the UE4 Editor to build. No patching or aditional pakages required.
- Unlimited MIDI file size using fast direct stream reading without using any MIDI reading librarys
- Much faster by using less performance compared to the original Pianofall
- Possible to change a lot using the Unreal Editor like Map and Object without even need to programm anything
- Possible to completely change everything with only very basic C++ programming knowledge
- Looks much better due to a modern and highly evolved game engine with amazing graphical effecs like Temporal AA
- Much more accurate physics using PhysX. UnrealPianofallLight even uses async sub-stepping for even more accuracy
- Rendering huge Black MIDIs on a non high-end PC (even a cheep/old one should be fine for a low blocklimit)
- Cross-Platform kompatible (just build it for any platform UE 4 suports)
- DirectX 10, 11, 12, OpenGL 3, 4 and Vulkan support!
- Predefined camera movement supported

### How to use
1. Render images using UnrealPianofallLight
2. Use http://en.cze.cz/Images-to-video to generate an wmv video
3. Use https://github.com/KaleidonKep99/Keppys-MIDI-Converter/releases to generate wav audio
4. Use https://www.ffmpeg.org/ (ffmpeg -i video.wmv -i audio.wav -codec copy -shortest output.wmv) to merge audio and video without any quality lose

Tips: To improve the audio rendered using Keppy's MIDI Converter I recommend http://www.audacityteam.org/download/ (Effect: Limiter -0.5 db) and then export it as floating-point wav file before merging audio and video. All images can also be edited before converting them to video using a Photoshop batch script and fast export it as png afterwards. The video file will be the same (same file hash) as by using the compressed but much slower png export.

## Command line arguments
Instead of a fancy GUI I decided to only use command line arguments so that you can easily put them into different batch files to optimize your productivity a lot. To make UnrealPianofall as customizable as Possible without the need for you to edit the source code I added as much as possible to hopefully cover any of your needs. If you miss one, or one isn't working properly just open an issue about it.

Excample how to call UnrealPianofall from CMD/PowerShell/BatchFile: .\UnrealPianofallLight.exe midi="C:/Users/nico/Music/BA_Rare_ASDF_Mode_rev_1.1.mid" --path="C:/Out/" --limit 250000 --reduction 6 --repeat --capture --res 2

Command line arguments (Light Version):
- --midi=\"filepath\" => the MIDI file (.mid) to open
- --path=\"dirpath\" => Screenshot save directory
- --res => Screenshot resol. multip. (default: 1.0) if not equals 1.0 --path will be ignored and the slower high resolution screenshot will be used even for multiplier values below 1.0!
- --PPQ => Overwrites the MIDI speed (pulses/quarter)
- --limit => The max amount of cubes in the scene
- --reduction => Max amount of cubes per frame per note
- --startframe => Skips the beginning of a MIDI file
- --block => 0=Piano key (1x10x10) (default), 1=Cube
- --block_x => Spawn X-position (default: 0.0)
- --block_y => Spawn y-position (default: 0.0)
- --block_z => Spawn Z-position (default: 5000.0f)
- --blockscale => Block size multiplier (default: 1.0)
- --material => 0=default, 1=basic (no reflections)
- --gravity => Sets the worls gravity (dafault: -980.0)
- --spawndist_x => Spawn X-offset multip. (default: 100)
- --spawndist_y => Spawn Y-offset multip. (default: 100)
- --width => Width res. (use together with --height)
- --height => Height res. (use together with --width)
- --mode => 0=Fullscreen, 1=WFull, 2=Wind. 3=NumWind.
- --audio => Enable MIDI out on the default MIDI channel
- --off => Enable sending MIDI off events to MIDI out
- --capture => Saves a screenshot of every frame
- --speed => Camera speed multiplicator (default: 1)
- --no-repeat => Don't repeat pre-defined camera movement
- --free => Disable camera and let you control the view
- --tp => Teleports the player to the blocks if --free
- --fix => Locks the camera after capture starts
- --fps => Limit the fps/speed to a specified framerate
- --vsync => Turn ON VSYNC (limits midi speed like --fps)
- --low => Default instead of the ultra graphic settings
- --wait => Seconds for world optimization before playing default: 1 seconds for loading the whole scene also useful in combination with --free in order to bring the camera in position befor capture start
- --help => Displays this help dialog box on launch

## How to build
1. Clon or download the UnrealPianofallLight sources
2. Open UnrealPianofallLight.uproject with Unreal Engine 4

## How to contribute
- Create Pull Requests (please choose the right branch)
- Report bugs, tell me your ideas or ask questions using GitHub's issue system (don't forget to mention that it occurred in the light version)
- Please never post any Engine- or marketplace content's source code because https://www.unrealengine.com/en-US/eula doesn't allow it but instead make patches which doesn't contain any such code like I did for ABoyandHisKite_patch.txt in the UnrealPianofall branch

Have fun!