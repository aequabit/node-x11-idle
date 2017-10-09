{
	"targets": [
		{
			"target_name": "x11-idle",
			"sources": [ "x11-idle.cpp" ],
			"link_settings": {
				"libraries": [
					"-lX11",
					"-lXext",
					"-lXss"
				]
			}
		}
	]
}
