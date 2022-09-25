Level0 = {
	assets = {
		[0] = { type = "texture", id = "jungle-tiletexture", file = "./assets/tilemaps/jungle.png" },
		[1] = { type = "texture", id = "terrain-texture-night", file = "./assets/tilemaps/jungle-night.png" },
		[2] = { type = "texture", id = "chopper-texture", file = "./assets/images/chopper-spritesheet.png" },
		[3] = { type = "texture", id = "tank-texture-big-left", file = "./assets/images/tank-big-left.png" },
		[4] = { type = "texture", id = "projectile-texture", file = "./assets/images/bullet-enemy.png" },
		[5] = { type = "texture", id = "radar-texture", file = "./assets/images/radar.png" },
		[6] = { type = "font", id = "charriot-font", file = "./assets/fonts/charriot.ttf", fontSize = 14 },
	},
	map = {
    textureAssetId = "jungle-tiletexture",
    file = "./assets/tilemaps/jungle.map",
    scale = 2,
    tileSize = 32,
    mapSizeX = 25,
    mapSizeY = 20,
	},
	entities = {
		[0] = {
			name = "player",
			layer = 4,
			components = {
				transform = {
					position = {
						x = 240,
						y = 106
					},
					width = 32,
					height = 32,
					scale = 1,
					rotation = 0
				},
				sprite = {
					textureAssetId = "chopper-texture",
					animated = true,
					frameCount = 8,
					animationSpeed = 8,
					fixed = false,
				},
				collider = {
					tag = "PLAYER"
				},
			}
		}
	}
}