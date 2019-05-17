# CloudSaver
## Requirements
- Windows OS
- a cloud software (for example the Dropbox desktop client)

## What does this program do?
- It allows you to play a singleplayer games on multiple devices without having to manually copy your saves from one to another
- How? Basically this program is an app-launcher with benefits:
  1. first it copies the save-files from your cloud cocation on your PC to wherever your game usually looks to find its saves
  2. it launches the actual game and waits for it to terminate
  3. the new save-files get copied back to the cloud location (and the cloud software should take care of uploading it)
- Paths can be edited in the `.ini` file, just launch the program once

## You don't like the additional ini file?
Sure thing, just remove the whole ini parsing and hardcode the paths/config parameters (lines 13-20). Should work just fine.

## Limitations
- this does not work on games that use a launcher themselves => if the .exe you specify terminates, the copy process will start
