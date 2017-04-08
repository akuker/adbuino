# Installation #
1. Install a webserver
2. Install PHP
3. Create a new folder called "up" in the same directory of index.php
4. `chmod 755 up`
5. That's it

# Configuration #
*   `$title`: Title of the page
*   `$filedir`: Directory where files will get uploaded
*   `$maxsize`: Maximus size (in bytes) of the uploaded file
*   `$allowedExts`: Allowed extensions
*   `$allowedMime`: Allowed mime types
*   `$baseurl`: Path of index.php

# License #
This code is released under the [WTFPL V2](http://www.wtfpl.net/ "WTFPL V2") license

# Demo #
A demo is avaiable [here](http://spittiepie.com/img/ "here")

# Warranty #
This project is released __without warranty__.  
I've coded this without prior knowledge of PHP in few hours, so it's probably full of bad habit.  
Don't complain with me if someone uses this to hack your server.  
You may want to disable PHP in the "up" folder, to increase security.  
Remember to set a size limit in your php.ini and/or your webserver too.
