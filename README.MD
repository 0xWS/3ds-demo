# Nintendo 3DS demo
Small demo for Nintendo 3DS to test out homebrew development. This is clone of a classic game Pong.
![screenshot](https://raw.githubusercontent.com/0xWS/3ds-demo/main/x.png)

## Compilling
Install [DevKitPro](https://devkitpro.org/wiki/Getting_Started) using their guide. You end up having Msys2 installed. Run its terminal, open the project folder and just run `Make`. You will end up with .3dsx file you can run in either Citra or real Homebrewed 3DS.

### t3x file generating
`tex3ds -i image.t3s -o output.t3x`
Move `output.t3x` to gfx folder. I can't figure how to make tex3ds work in Makefile for now.

## Running
### Citra
For [Citra](https://citra-emulator.com/) just open the `.3dsx` file and it should work.

### Real 3DS
Move both romfs folder and the `.3dsx` file to the `3ds` folder on your SD card. You will be able to run the example through the Homebrew launcher

## Playing
Left player is controlled by UP/DOWN on DPAD, right player is controlled by X/B. The game is endless.