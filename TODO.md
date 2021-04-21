# **Contents**

- [**Contents**](#contents)
- [**TODO**](#todo)
- [**BUG**](#bug)
- [**Identified**](#identified)



# **TODO**

<span style="font-size:2em;">

**work on gui**

</span>

 * - [X] add features of core to gui menu (add, del, find, help....)
        
   * - [x] use a tab dialog to show user about missing configs & help them to complete config
      * - [x] add (In Progress)
      * - [x] del.
      * - [x] find
    
    * - [x] cached wallpaper display in a separate dlg (w/ QImage lib)
    
    * - [X] simple image editing QDialog
      * - [X] image cropping & rotation add to mainwindow (w/ QImage, QTransformation)

 * - [X] optimize program UI & relaunch & launch at startup
    
   * - [X] Improve UI Layout
   * - [X] Improve UI hotkeys & shortcuts
    
   * - [X] Add crash relaunch
   * - [X] Add auto launch at startup
    
 * - [ ] optimize program
    
   * - [ ] smart ListView reloading in CacheBrowserDlg, only reload ListViews that need to reload
   * - [ ] use pipe for communication between gui & core?


# **BUG**

    J. Unsure issue: [gui] will crash after running for a long time.
    After running for a long time, gui will take less memory,
    but will crash when update_wallpaper. And later, it will continue
    crashing when program start This may due to uncleared ./core_cache file
    
    K. [gui] Will potentially crash somehow
             Cannot display gui correctly, after a long time of running
          	 And will crash

# **Identified**

    G. [gui] program mem usage may continue add-up w/ update_wallpaper()
    This only happens when rapidly calling update_wallpaper().
    Windows will temporarily cache something to program's running memory
    Those cached memory will be automatically remove later.
    
    H. [gui] use a lot of memory after open file selecting dialog.
    Windows will automatically cache after 1st time open a file
    selecting dialog for faster opening later.
    This happens in notepad and softwares as well.
    Or, QProcess in update_wallpaper() will last for a while after
    function finished because it's running async.
    
    L. [gui] CacheBrowserDlg will crash when trying to remove a cached wallpaper
             while there is already many cached wallpaper in it.
    
    M. Program cannot launch at startup properly. Previously, we achieve launch at startup
       through Registry, but it requires program be have admin privilege. Right now, we
       achieve launch at startup through placing an app shortcut in Current User Startup Folder.
       This method would not require admin privilege.
