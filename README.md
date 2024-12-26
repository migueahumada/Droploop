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