# Droploop

Libraries in this project:
- libsndfile
- Dear ImGui
- GLFW
- OpenGL3

# Structure
- AudioLoader
	- Use libsndfile to load audio files
	- AudioLoader singleton

- AudioPlayer
	- Use Portaudio to play audio files
	- AudioPlayer that communicates between portaudio and Audio Loader
- Clases:
	- Loader and deloader de audio -> AudioLoader
	- IO de audio -> IOAudio
	- Efectos de audio -> DSP
	- Mezclar audio ->  Mixer
	- Clase para definir el ente de audio -> SoundFile 
	

- AudioLoader -> Obtener la metadata de los archivos de audio con libsndfile.
- AudioIO -> Para poder iniciar streams de audio y poder reproducirlos con portaudio.
- FileLoader -> Para buscar archivos de audio en el sistema de archivos.
- Application -> GUI para que el usuario pueda interactuar con el programa.