Introduction
============

VxKex is a set of API extensions for Windows 7 that allow some Windows 8,
8.1 and 10-exclusive applications to run on Windows 7.

To download and install, see the [releases page](https://github.com/CopperFr/VxKex/releases).

After installation, usage is easy: just right click on a program (.exe or .msi),
open the Properties dialog, and select the VxKex tab. Then, check the check box
which says "Enable VxKex for this program", and try to run the program. You can also
apply it on a shortcut (it will automatically update configuration on the target)

Some programs require additional configuration. There's a link file called **"Application
Compatibility List"** inside the VxKex installation folder (which is C:\Program
Files\VxKex by default) which will open the [Application Compatibility List](https://github.com/CopperFr/VxKex/wiki/Application-Compatibility-List) wiki page details these steps, but for the most part, all
configuration is self-explanatory.

FAQ
===

**Q: Does it work for games???**

**A:** At the moment, VxKex is not intended for games, so you will probably have limited
success. I hope to address this shortcoming in a future release.

**Q: What applications are supported?**

**A:** The list of tested compatible applications includes, but is not limited to:
- Android Studio Meerkat 2024.3.2 Patch 1
- Antigravity 1.11.17
- Blender 4.5.6 / 5.0.1
- Chrome 138.0.7204.97
- Firefox 128.9.0esr
- Firefox 130.1
- GitHub CLI 2.78.0
- GitHub Desktop 3.5.3
- GIMP 3.0.2-1
- Godot 4.5.1.stable
- GoLang 1.25
- Inkscape 1.4.2
- Opera 123.0.5669.47
- PowerShell 7.5.4
- Python 3.14.2
- Rust 1.90.0
- Spotify 1.2.61.443
- Vivaldi 7.6.3797.63
- VSCode 1.107.1
- VSCodium 1.107.18627
- WinDbg Classic 10.0.2600.1

See [Application Compatibility List](https://github.com/CopperFr/VxKex/wiki/Application-Compatibility-List), for more information on configuration needed for each tested application.

The majority of Qt6 applications will work, and many Electron applications will
work as well.

**Q: Does VxKex modify system files? Will it make my system unstable?**

**A:** VxKex does not modify any system files. Its effect on the whole system is
extremely minimal. No background services are used, no global hooks are
installed, and the shell extensions and DLLs that are loaded have minimal
impact and can be disabled if needed. You can rest assured that your Windows 7
will remain as stable as it always is.

**Q: Do I need to have specific updates installed?**

**A:** VxKex is only designed to work with Service Pack 1 installed. Users of
Windows 7 RTM can try to use it, but I don't know if it will install or work.
Many programs require KB2533623 and KB2670838 in order to run. It is a good
idea to install those two updates.

**Q: If I have ESUs (Extended Security Updates) installed, can I use VxKex?**

**A:** Yes. There is no problem with ESUs.

**Q: Do console applications work with VxKex?**

**A:** Yes. After you have enabled VxKex for a program you can use it through the
command prompt as normal.

**Q: Can I use this with Windows 8 or 8.1?**

**A:** VxKex is designed for use only with Windows 7.

**Q: How does VxKex work?**

**A:** VxKex works by loading a DLL into each program where VxKex is enabled. This
is accomplished through using the IFEO (Image File Execution Options) registry key.

Specifically, the "VerifierDlls" value is set to point to a VxKex DLL. This DLL then
loads into the process.

API extension is accomplished by editing the program's DLL import table so that
instead of importing from Windows 8/8.1/10 DLLs, it imports to VxKex DLLs instead.
These VxKex DLLs contain implementations of Windows API functions which were introduced
in newer versions of Windows.

**Q: I've an error message "xxx.exe is not a valid Win32 application"**

**A:** The BHO CpiwBypa and option to activate have been removed (was not working very well) but VxKex (as minimal VxKex) can be
activated on explorer.exe instead:

![VxKex configuration GUI](/explorer-properties-screenshot.png)

You need to check **Disable VxKex for child process** on explorer.exe to avoid that all applications run with VxKex enabled.

After each explore.exe settings changes a message will prompt you to restart:

![VxKex configuration restart](/restart-explorer-screenshot.png)
