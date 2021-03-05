# WallpaperChanger

<br>

<span style="font-size:2em;">

**Currently This Program Is Only Designed For Windows**

</span>


## What It Does:

This Program will Change Wallpaper base on Screen Rotation on tablet/laptop. This program is aiming to allow user to customize their wallpapers base on different screen orientation. You can set different wallpaper for different orientation.

## [**Take me to Installation!**](#Requirements)

## [**Go To TODO.md**](TODO.md)

<br>

## How It Work:

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
There 4 files in this directory tree, and:
* **TranscodedWallpaper** is a **Compressed Version** of original user-defined wallpaper (or default wallpaper)
  * Windows will generate all the wallpapers from this file
  * The quality of this file will affect all the auto-generated wallpapers 
* slideshow.ini records user's wallpaper slideshow information
* Directory **CachedFiles/ will Only Be Created When System Is In Modes Beside Extend (This PC Only, Duplicate, Second Screen Only)**
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
* Program will automatically replace **./TranscodedWallpaper** with a **Default Wallpaper** that choosed by user
* If Windows is in **Extended Mode**: Program will let Windows to generate wallpapers for all the displays from **Default Wallpaper** choosed by user
* If Windows is in **Other Modes**: Program will replace files under **CachedImage/** with **Landscape Wallpaper** and **Portrait Wallpaper** that choosed by user
  * Windows will use replaced wallpapers in CachedImage/ as your wallpapers without any problems


<br>

## Requirements:

* **C++17** and above
* **Qt 5** and above
* **CMake 3.18** and above

<br>

## Building

* Use CMake to build the project
```
    Commands for Reference:
    
    ~> $ mkdir build 
    
    ~> $ cd build 
    
    ~/build> $ cmake .. 
```
* After Installation, run **WallpaperChanger-gui**
* If you are **missing DLLs**, you can try to download them from: [https://www.dll-files.com/](https://www.dll-files.com/) 

<br>

## Further Explanation On GUI

* **Default Walpaper**: Wallpaper that choose by user and use to set as Windows system wallpaper
* **Landscape Wallpaper**: Wallpaper that choose by user and will display on landscape orientation in **Single Screen Modes**
* **Portrait Wallpaper**: Wallpaper that choose by user and will display on portrait orientation in **Single Screen Modes**
* **Wallpaper Update Interval**: How long to update (replace) **./TranscodedWallpaper** and files under **./CachedImage/**
  * This value will store as seconds in config.ini
  * Menu Option **Update Wallpaper** will do the same update immediately
* **Hide Program When Closed**: Toggle what will "X" on the upper-right corner does
