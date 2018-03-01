DMGMaker = ./tools/create-dmg
ICON = ./tools/WAH.icns
DMG_Background = ./tools/WAH.icns
DMGSource = ./release

dmg: $(DMGSource)
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
	WallsAndHoles.dmg \
	$(DMGSource)

clean:
	rm WallsAndHoles.dmg