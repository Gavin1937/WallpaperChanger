# WallpaperChanger


<span style="font-size:2em;">

**Currently This Program Is Only Designed For Windows**

</span>


## What It Does:

This Program will Change Wallpaper base on Screen Rotation on tablet/laptop. This program is aiming to allow user to customize their wallpapers base on different screen orientation. You can set different wallpaper for different orientation.

## [**Take me to Installation!**](#Requirements)

## [**Go To TODO.md**](TODO.md)

## [**Go To Changelog.md**](Changelog.md)


## How It Works:

Windows will automatically store its wallpaper to this directory
```
C:\Users\%UserName%\AppData\Roaming\Microsoft\Windows\Themes
```
Here is an example directory
```
    Themes/
        |
        ----- CachedFiles/
        |           |
        |           ----- Landscape_Wallpaper.jpg
        |           |
        |           |
        |           ----- Portrait_Wallpaper.jpg
        |
        |
        ----- TranscodedWallpaper
        |
        |
        ----- slideshow.ini
```
There are 4 files under this directory tree, and:
* **TranscodedWallpaper** is a **Compressed Version** of original user-defined wallpaper (or default wallpaper)
  * Windows will generate all its wallpapers from this file
  * The quality of this file will affect all the auto-generated wallpapers 
* slideshow.ini records user's wallpaper slideshow information
* Directory **CachedFiles/ will Only Be Created When System Is In Any Modes Beside Extend (This PC Only, Duplicate, Second Screen Only)**
  * Windows Will Automatically remove this directory when it is in Extended Mode
  * Files Under this directory will named as following:

```
CachedImage_${H}_${V}_POS${M}
    
    or
    
CachedImage_${V}_${H}_POS${M}

    ${H} => Screen Horizontal Resolution
    ${V} => Screen Vertical Resolution
    ${M} => Wallpaper Mode:
            0 = Center
            1 = Tile
            2 = Stretch
            3 = Fit
            4 = Fill
            5 = Span
```
* * All the files in this directory is auto generate generate from **./TranscodedWallpaper**
* This program currently **Only designed for Fill Mode**
* Program will automatically replace **./TranscodedWallpaper** with a **Default Wallpaper** that chose by user
* If Windows is in **Extended Mode**: Program will let Windows to generate wallpapers for all the displays from **Default Wallpaper** chose by user
* If Windows is in **Other Modes**: Program will replace files under **CachedImage/** with **Landscape Wallpaper** and **Portrait Wallpaper** chose by user
  * Windows will use replaced wallpapers in CachedImage/ as your wallpapers without any problems


## Requirements:

* A **Windows** Tablet or Laptop with screen rotation enabled
* **C++17** and above
* **Qt 6**
* **CMake 3.18** and above


## Building

* Use CMake to build the project
```
    Commands for Reference:
    
    ~ $ mkdir build 
    
    ~ $ cd build 
    
    ~/build $ cmake .. 
```
* After Building, run **WallpaperChanger-gui.exe**
* If you are **missing DLLs**, you can try to download them from: [https://www.dll-files.com/](https://www.dll-files.com/) 


## Tips About GUI

* **Default Wallpaper**: Wallpaper that choose by user and use to set as Windows system wallpaper
* **Landscape Wallpaper**: Wallpaper that choose by user and will display in landscape orientation in **Single Screen Modes**
* **Portrait Wallpaper**: Wallpaper that choose by user and will display in portrait orientation in **Single Screen Modes**
* **Wallpaper Update Interval**: How long to update (replace) **./TranscodedWallpaper** and files under **./CachedImage/**
  * This value will store as seconds in config.ini
  * Menu Option **Update Wallpaper** will do the same update immediately
* **Hide Program When Closed**: Toggle what will "X" on the upper-right corner does
* There is a **unsaved change protection** build into the program in order to prevent exceptions. Just be sure to fill in all 3 Wallpapers and save changes.
* **Keyboard & Mouse Shortcuts**
  * **CacheBrowser**
    * **Mouse Left Button Double Click**
      * If selected item is **Missing Wallpaper**, Select Wallpaper From computer
      * If selected item is **Wallpaper**, Open selected Wallpaper in **ImageEditor**
    * **Mouse Right Button Click** - Open operation menu on selected Wallpaper
    * **Ctrl + D, Del** - Delete selected Wallpaper
    * **Alt + 1** - Move selected Wallpaper to **Default Wallpaper Section**
    * **Alt + 2** - Move selected Wallpaper to **Landscape Wallpaper Section**
    * **Alt + 3** - Move selected Wallpaper to **Portrait Wallpaper Section**
  * **ImageEditor**
    * **Mouse Right Button Click** - Open operation menu
    * **Arrow Keys** - Move Cropping Rectangle around the screen with speed of 1 pixel/frame, this feature is designed to help user perform  micro-adjusting.


## Resources Used

* Program Icon and Application Icon are come from [凪白みと's Artwork](https://www.pixiv.net/artworks/70352399)
* All Icons in ImageEditor are come from [www.flaticon.com](www.flaticon.com)
* **All Icons Are Not For Commercial Use**

