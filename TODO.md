# **Contents**

- [**Contents**](#contents)
- [**TODO**](#todo)
- [**BUG**](#bug)
- [**Identified**](#identified)



# **TODO**

<span style="font-size:2em;">

**work on gui**

</span>

 * - [ ] add features of core to gui menu (add, del, find, help....)
        
   * - [x] use a tab dialog to show user about missing configs & help them to complete config
      * - [x] add (In Progress)
      * - [x] del.
      * - [x] find
    
    * - [ ] image cropping & rotation add to core (w/ QImage lib)
    
    * - [x] cached wallpaper display in a separate dlg (w/ QImage lib)
    
    * - [ ] simple image editing QDialog


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
