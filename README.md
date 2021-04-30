# brokenithm-kb 💸

> Because $300 plus shipping and a two month wait is too painful for a tasoller

Run this together alongside SUSPlayer or your other favourite sliding universal score simulator.

## Setup

1. Check the releases page on github, download and unzip into your preferred folder.
2. Connect an iPad or some other tablet device to the **same wifi access point** as your windows machine.

   > Windows hotspot also works, just make sure you turn on the hotspot before running `brokenithm-kb.exe`.

   ![hotspot](images/wifi.png)

3. Run `brokenithm-kb.exe`.
4. If this is the first time running the program, give the program firewall permissions.

   ![firewall](images/firewall.png)

   > When using windows hotspot, ensure 'public' is enabled. If there are any other issues, try checking the "incoming rules" menu of windows firewall from the control panel.

   > You may need grant firewall permissions the first time and then restart the program.

5. Some URLs should be displayed on the command line window, try opening each one of them in your tablet device until you see the controller screen.

   ![running window](images/link.png)

6. If you see 'Controller ID ? Connected', the controller is working!

   ![connected](images/connected.png)

7. Open up SUSplayer or some other simulator and have fun!

I also recommend using the "Add to home" feature for Safari / Chrome, it makes the controller fullscreen.

![add to home](images/homescreen.png)

The difference between Safari and "Add to home" looks like this:

![safari vs add to home](images/fullscreen.png)

## Options

Options for the server can be changed with command line arguments, check `.\brokenithm-kb.exe -h` or edit `run.bat`.

Appearance of the touchscreen controller can be changed by editing `./res/www/config.js`.
You can change the displayed colors and also add a background image URL to the controller. Just upload an image to imgur or somewhere else and place the link there.

## Building from source

Needs cmake and some vcpkg packages:

```
libuv:x64-windows
openssl-windows:x64-windows
openssl:x64-windows
usockets:x64-windows
zlib:x64-windows
```

Built on windows `cl.exe 19.28.29337`.

If anyone knows enough C++/cmake/CI to help out with making this section better do pm me.

## Attribution

[uWebsockets](https://github.com/uNetworking/uWebSockets) is licensed under the Apache License 2.0.

[spdlog](https://github.com/gabime/spdlog) is licensed under the MIT License.

Packaged background image was created using [SVG-Backgrounds](https://www.svgbackgrounds.com/).

[Twemoji](https://github.com/twitter/twemoji) is licensed under CC-BY 4.0.
