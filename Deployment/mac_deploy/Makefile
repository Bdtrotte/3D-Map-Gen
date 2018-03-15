DMGMaker = ./tools/create-dmg
DEPLOYFIX = ./tools/macdeployqt
ICON = ../../WallsAndHoles/images/wahicon.icns
DMGSource = ./release

dmg: $(DMGSource)
	cp ./tools/example.Info.plist $(DMGSource)/WallsAndHoles.app/Contents/Info.plist
	$(DMGMaker) \
	--volicon $(ICON) \
	--volname "WallsAndHoles Installer" \
	--window-pos 200 120 \
	--window-size 400 400 \
	--icon-size 100 \
	--icon WallsAndHoles.app 100 100 \
	--hide-extension WallsAndHoles.app \
	--app-drop-link 200 200 \
	WallsAndHoles.dmg \
	$(DMGSource)

clean:
	rm WallsAndHoles.dmg