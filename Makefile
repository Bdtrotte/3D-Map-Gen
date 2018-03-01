DMGMaker = ./tools/create-dmg
ICON = ./tools/W.icns
DMG_Background = ./tools/W.icns
Release = ./build-release

dmg: ./dmg-release
	$(DMGMaker) \
	--volicon $(ICON) \
	--background $(DMG_Background) \
	--volname "WallsAndHoles Installer" \
	--window-pos 200 120 \
	--window-size 400 400 \
	--icon-size 100 \
	--icon WallsAndHoles.app 100 100 \
	--hide-extension WallsAndHoles.app \
	--app-drop-link 200 200 \
	$(Release)/WallsAndHoles.dmg \
	./dmg-release

clean:
	rm $(Release)/*