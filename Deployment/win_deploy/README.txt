The installer already in this folder should be good to install the latest version
of WallsAndHoles, but if it doesn't work for some reason, or you need a more up to
date version, one can follow these instructions to build a new installer.

To create an installer for windows, one should use the following steps:
1.) Build the project using Qt
2.) Locate the build directory, and delete all files, except the .exe
3.) Use the apropriate version of windeployqt.exe on the built WallsAndHoles.exe
    This will produce all dependancies which need to be included. It would be good
    at this phase to test the .exe and make sure all works well.
4.) Move all files into a 7zip archive. The qt installer framework include a tool
    to do this: "archivegen.exe"
5.) Move the zip to packages/com.wah.product/data/
    It should be the only file in this folder.
6.) From the directory containing this file, fun the following command:

	<location-of-ifw>\binarycreator.exe -t <location-of-ifw>\installerbase.exe -p packages -c config/config.xml WallsAndHolesInstaller


Notes:
"windeployqt.exe" can be found in .../Qt/Qt<Version>/<Version>/<Correct Distribution>/bin