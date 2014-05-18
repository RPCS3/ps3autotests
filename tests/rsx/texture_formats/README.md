Before you run the test, make sure you run the `Texture_Generate.py` in order to generate all the remaining .RAW textures that will be used by the sample. In the end, following files should be present along the binary:
* *Texture-B8.raw*
* *Texture-A1R5G5B5.raw*
* *Texture-A4R4G4B4.raw*
* *Texture-R5G6B5.raw*
* *Texture-A8R8G8B8.raw*
* *Texture-COMPRESSED_DXT1.raw*
* *Texture-COMPRESSED_DXT23.raw*
* *Texture-COMPRESSED_DXT45.raw*
* *Texture-G8B8.raw*
* *Texture-R6G5B5.raw*
* *Texture-DEPTH24_D8.raw*
* *Texture-DEPTH24_D8_FLOAT.raw*
* *Texture-DEPTH16.raw*
* *Texture-DEPTH16_FLOAT.raw*
* *Texture-X16.raw*
* *Texture-Y16_X16.raw*
* *Texture-R5G5B5A1.raw*
* *Texture-COMPRESSED_HILO8.raw*
* *Texture-COMPRESSED_HILO_S8.raw*
* *Texture-W16_Z16_Y16_X16_FLOAT.raw*
* *Texture-D1R5G5B5.raw*
* *Texture-D8R8G8B8.raw*
* *Texture-Y16_X16_FLOAT.raw*
* *Texture-COMPRESSED_B8R8_G8R8.raw*
* *Texture-COMPRESSED_R8B8_R8G8.raw*
	
This test is a 5x5 grid with following texture formats:
* `D1R5G5B5`, `D8R8G8B8`, `Y16_X16_FLOAT`, `COMPRESSED_B8R8_G8R8`, `COMPRESSED_R8B8_R8G8`
* `Y16_X16`, `R5G5B5A1`, `COMPRESSED_HILO8`, `COMPRESSED_HILO_S8`, `W16_Z16_Y16_X16_FLOAT`
* `DEPTH24_D8`, `DEPTH24_D8_FLOAT`, `DEPTH16`, `DEPTH16_FLOAT`, `X16`
* `COMPRESSED_DXT1`, `COMPRESSED_DXT23`, `COMPRESSED_DXT45`, `G8B8`, `R6G5B5`
* `B8`, `A1R5G5B5`, `A4R4G4B4`, `R5G6B5`, `A8R8G8B8`

White/Black squares are formats that are not yet included in the test.
