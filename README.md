<a name="readme-top"></a>
# Find Your Way Out (Console)

<!-- Screenshots -->
<p align="center">
  <img src="/Screenshot/screenshot_1.jpg?raw=true" />
  <img src="/Screenshot/screenshot_2.jpg?raw=true" />
</p>

<!-- TABLE OF CONTENTS -->
<summary>Table of Contents</summary>
<ol>
  <li>
    <a href="#about-the-project">About The Project</a>
    <ul>
      <li><a href="#description">Description</a></li>
      <li><a href="#built-with">Built With</a></li>
    </ul>
  </li>
  <li>
    <a href="#getting-started">Getting Started</a>
    <ul>
      <li><a href="#prerequisites">Prerequisites</a></li>
      <li><a href="#installation">Installation</a></li>
      <li><a href="#play-the-game">Play the game</a></li>
    </ul>
  </li>
  <li><a href="#components-of-the-project">Components of the project</a>
    <ul>
      <li><a href="#keybindings">Keybindings</a></li>
      <li><a href="#renderer">Renderer</a></li>
      <li><a href="#local-files">Local files</a></li>
      <li><a href="#objectmanager">ObjectManager</a></li>
      <li><a href="#raytracer">Raytracer</a></li>
    </ul>
  </li>
  <li><a href="#contributing">Contributing</a></li>
  <li><a href="#license">License</a></li>
  <li><a href="#contact">Contact</a></li>
</ol>
  
<!-- ABOUT THE PROJECT -->
## About The Project
Here are all the main informations about the project.

### Description
This project is more a private project for me to learn and practice C++ in a project. It will not be released or published at all.

### Built With
The project uses only libraries that are available on the system itself. No third party libraries will be used.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started
In this section is described how to start using this project to develop it and what is required to do so.

### Prerequisites
To edit and build the project Visual Studio on Windows is required currently because of the solution file.<br>
To clean the project from Visual Studio temporary files execute Clean.bat.

### Installation
1. Clone the repo
```sh
git clone https://github.com/VRSimDude/FindYourWayOut.git
```
2. open the project<br>
open FindYourWayOut.sln

### Play the game
To play the game built it in Visual Studio or start it directly from Visual Studio.<br><br>
You can also start the game without Visual Studio by download the executables for windows here:<br>
[v1.0.0 Win64](https://github.com/VRSimDude/FindYourWayOut/releases/download/v1.0.0/Executable.win64.zip)<br>
[v1.0.0 Win32](https://github.com/VRSimDude/FindYourWayOut/releases/download/v1.0.0/Executable.win32.zip)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- COMPONENTS OF THE PROJECT -->
## Components of the project
Here is described how you use the projects features.

### Keybindings
Move the player with WASD.
Exit a level with Escape.

### Renderer
The renderer is responsible for rendering the pixels on the screen.<br>
In the Init() function the size of the console can be changed.
Also the size of the font can be changed. But be aware that characters are rendered at this size too.
So if you change that to one pixel every char is only one pixel and not readable anymore.<br><br>
The renderer also change the console during the game but reset it as it was before the game was started.<br><br>
It has 2 overloaded functions to render something on the screen.<br>
```sh
RenderAtPos(const WCHAR& _unicodeChar, const WORD& _attribute, const SVector2& _pos)
```
With this function a single pixel can be drawn at the screen at the given position.

```sh
RenderAtPos(CHAR_INFO const* const _pSprite, const SVector2& _pos, int _width, int _height)
```
With this function a sprite can be drawn on the screen starting at the given position.

### Local Files
There are 2 local files that can be used to change the game.
You can change these however you like.
Please be aware that it always needs a player and an exit for every level.<br><br>

#### level_X.cfg
In these files every single level can be configured differently.<br>
Here is an example of the first level config (level_1.cfg):<br>
<p align="center">
	<img src="/Screenshot/screenshot_3.jpg?raw=true" />
</p>
The key value pairs in that file should explain every config by itself.<br>
The degree values like Camera FOV are in degree.<br>
The distance and height values are in cm.<br>
The time is in seconds.<br>
If this level is the last level, last_level must be set to 1 or anything but not 0<br>
The char values are used in the level files to determine which chars are used for which class

#### level_X.map
These files are used for the actual architecture of the map.
It uses the chars from the config file to determine which char is what class.
Empty chars are used for normal ground and ceiling.<br>
Here is an example of the first level (level_1.map):<br>
<p align="center">
	<img src="/Screenshot/screenshot_4.jpg?raw=true" />
</p>

### ObjectManager
The object manager is a singleton calls which handles every object in the level.
It is responsible for loading the level config and map from file and create the necessary objects.
Deleting the objects are also handled by the object manager.
It also has a functionality to check wether a given location hits a wall or not.<br><br>
To prevent the game for handling too many object, the level is split in chunk of objects.
During the level loading the object manager splits the level in chunks which are the size of the camera max view distance from the config file Local files above).<br>
Only the objects in the chunks around the chunk where the player currently is are updated and used for collision detection.
So in total there are 9 chunks to check.<br>
<p align="center">
	<img src="/Screenshot/screenshot_5.jpg?raw=true" />
</p>

### Raytracer
The raytracer is responsible for the calculation of the distance to a wall or the exit location for the whole screen.<br>
Here is a simplified version of the workflow of how this works:<br>
<p align="center">
	<img src="/Screenshot/screenshot_6.jpg?raw=true" />
	<img src="/Screenshot/screenshot_7.jpg?raw=true" />
</p>
<ol>
  <li>
    The starting angle is calculated by the camera angle and half of the horizontal field of view.
    The degree for every horizontal pixel is calculated, so that the field of view is displayed along the horizontal pixels of the screen.
  </li>
  <li>
    The forward vector for the current trace angle is calculated (1. line forward in image).
  </li>
  <li>
    Then the 2 vector for the axis are calculated to the nearest wall (horizontal and vertical check triangle in image).
  </li>
  <li>
    Now the vector for a single step horizontal or vertical is calculated (horizontal and vertical check every step in image).
    This works only in the was it does because the level layout is grid based.<br>
  </li>
  <li>
    Now the shorter of the nearest to wall vector of horizontal or vertical (horizontal and vertical check triangle in image) is used to check for collision.
  </li>
  <li>
    If there is no collision this location is increased by the horiztontal or vertical for one step (horizontal and vertical check every step in image).<br>
    Now again the shorter vector of the two is used to check for collision.<br>
  </li>
  <li>
    This is done util a wall is hit or the max view distance is reached.
  </li>
  <li>
    If there is no collision the vertical line on the screen is only filled with the ground and ceiling color
  </li>
  <li>
    If there is a collision the height to the field of view vertical and the collision location is calculated (FOV height at wall location in image).
  </li>
  <li>
    The the wall percentage of that height is calculated (the wall is 200cm height).
    This percentage will the be used to calculate the start position of the wall in pixel of the screen height.
    Because the camera is in the middle of the screen the height of the wall is calculated.
    The closest starting pixel of the wall to the middle of the screen is saved.
  </li>
  <li>
    Now the wall and the ceiling and ground is drawn at the screen.
  </li>
  <li>
    Now the current trace angle is increased by one step (from calculation step 1).
    The steps 2 to 11 are now repeated for every horizontal line on the screen.
  </li>
  <li>
    After every line was drawn there is a check wether any starting pixel of a wall is closer to the middle than 40% of the screen height.
    All lines that have a dark green pixel there will be colored to a green pixel.
    Therefore, the green color is always until the lowest wall pixel.
  </li>
</ol>

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTRIBUTING -->
## Contributing

Contributions to this project can be done by writing me.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the MIT License License. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

mail: [contact@marcfriedrich.com](mailto:contact@marcfriedrich.com)<br>
project link: [https://github.com/VRSimDude/FindYourWayOut](https://github.com/VRSimDude/FindYourWayOut)

<p align="right">(<a href="#readme-top">back to top</a>)</p>
