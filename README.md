The game engine

I. CREATE VISUAL STUDIO PROJECT
	1. Create new visual studio dll empty project
	2. Add source files from src/include, src/src directories
	3. Open project Properties -> Configuration Properties:
	3.1. Open General:
	3.1.1. Set Output Directory to ..\..\src\bin (path to engine_directory\src\bin)
	3.1.2. Set Target Directory to .dll
	3.1.3. Set Configuration Type to Dynamic Library (.dll)
	3.1.4. Set Character Set to Not Set
	3.2. Open Debugging:
	3.2.1. Set Working Directory to ..\..\src\bin (path to engine_directory\src\bin)
	3.3 Open C/C++ -> General
	3.3.1. Additional Include Directories: 
			- DIRECTX_SDK_INCLUDE_PATH
			- CG_SDK_INCLUDE_PATH
			- ..\..\src\include (path to engine_directory\src\include)
	3.3.2. Multi Procesor Compilation: /MP

II. Run sample manager
	1. Add QTSDK system environment variable
	2. Copy QT dlls to bin directory with SampleManager executable
	3. Install Visual Studio 2012 redist
	4. Put those QT libs to the bin directory: libEGL
	5. Copy platforms directory to the bin directory
	6. Copy DevIL.dll into the bin directory
	7. Copy DirectX dll into the bin directory