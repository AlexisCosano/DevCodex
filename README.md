# Monster Heights
2D platform game made with my own 2D game engine.
Made by [Alexis Cosano](https://github.com/AlexisCosano).

IMPORTANT: in some laptops, the game doesn't display well due to the laptop's resolution (or maybe you feel like you're missing something from the environment). If that's the case, open the config.xml, and under window -> options, change resizable="false" into resizable="true". Once you open the game, maximize the window, and it should display everything.

Keep in mind that you will see the black background and some more imperfections as the game hasn't been designed to show those parts, but at least it will display everything correctly if the resolution is low.

Link to the repository: https://github.com/MrAlcoro/DevCodex


----------------------------------------------------------------------

Controls:

- A = Move left
- D = Move right
- Space = Jump

- Repag = Increase music's volume
- Avpag = Decrease music's volume

When in God mode:

- A = Move left
- W = Move up
- S = Move down
- D = Move right

To kill the enemies: you need to be falling down on them and you will not bounce off of them, you will fall down as you'do performing a regular jump.

----------------------------------------------------------------------

Debug options:

- F1 = Load first map
- F2 = Load second map
- F3 = Restart the current map
- F5 = Save current state
- F6 = Load saved state
- F9 = Debug mode
- F10 = God mode
- F11 = Alternate between FPS capped at 30 or 60

Goal = get to the end of the level, it's lineal so, you'll eventually find it. (Hint: if you see boxes, it means that the level is coming to an end!)

----------------------------------------------------------------------

Known issues:

- The parallax creates some minor artifacts in some specific cases.
- There are no sound effects (yet!).
- You cannot win while using God mode at the moment.
- It's lacking feedback, but it will improve soon!
- The enemies don't respawn when the player dies, only when the level resets.
- There is no grounded enemy.
- The flying enemy doesn't have animations.
- DO NOT resize or move around the game's window. If you do so, the player will fall endlessly and you will have to reset the level.
- The enemies' states are currently not saved.
- The enemies and the environment can start vibrating on specific spaces of the level.

----------------------------------------------------------------------

Credits: 

Music used: Mimiga Town, from Cave Story. I do not own this song, Cave Story is owned by Studio Pixel.

The sprites used come directly from our own games, made by Martí Pinós Melo and Sergi Pérez Crespo.

--------------------------------------------------------------------------
MIT License

Copyright (c) 2019 Alexis Cosano Rodríguez 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
