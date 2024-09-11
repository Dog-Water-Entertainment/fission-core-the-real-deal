//Finally a straight forward OpenAL sample program.
//Gordon Griesel
//2016
//This demo plays two simultaneous sounds.
//One is looping, the other is not looping.
//
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //USE_OPENAL_SOUND

int main()
{
	//Get started right here.
#ifdef USE_OPENAL_SOUND
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		return 0;
	}
	//Clear error state.
	alGetError();
	//
	//Setup the listener.
	//Forward and up vectors are used.
	//The person listening is facing forward toward the sound.
	//The first 3 components of vec are 0,0,1
	//this means that the person is facing x=0, y=0, z=1, forward.
	//The second 3 components means that up is x=0,y=1,z=0, straight up!
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	//
	//Buffers hold the sound information.
	ALuint alBuffer[2];
	alBuffer[0] = alutCreateBufferFromFile("./test.wav");
	//let's create a looping sound.
	alBuffer[1] = alutCreateBufferFromFile("./737engine.wav");

	//
	//Source refers to the sound.
	ALuint alSource[2];
	//Generate 2 sources, and store in the matching buffers.
	alGenSources(2, alSource);
	alSourcei(alSource[0], AL_BUFFER, alBuffer[0]);
	alSourcei(alSource[1], AL_BUFFER, alBuffer[1]);
	//
	//FirstSet volume and pitch to normal, no looping of sound.
	alSourcef(alSource[0], AL_GAIN, 1.0f);
	alSourcef(alSource[0], AL_PITCH, 1.0f);
	alSourcei(alSource[0], AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return 0;
	}
	//now a looping sound... jet engine.
	alSourcef(alSource[1], AL_GAIN, 0.5f);
	alSourcef(alSource[1], AL_PITCH, 1.0f);
	alSourcei(alSource[1], AL_LOOPING, AL_TRUE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return 0;
	}
	alSourcePlay(alSource[1]);
	for (int i=0; i<42; i++) {
		alSourcePlay(alSource[0]);
		usleep(500000);
	}
	//Cleanup.
	//First delete the sources.
	alDeleteSources(1, &alSource[0]);
	alDeleteSources(1, &alSource[1]);
	//Delete the buffers.
	alDeleteBuffers(1, &alBuffer[0]);
	alDeleteBuffers(1, &alBuffer[1]);
	//
	//Close out OpenAL itself.
	//Get active context.
	ALCcontext *Context = alcGetCurrentContext();
	//Get device for active context.
	ALCdevice *Device = alcGetContextsDevice(Context);
	//Disable context.
	alcMakeContextCurrent(NULL);
	//Release context(s).
	alcDestroyContext(Context);
	//Close device.
	alcCloseDevice(Device);
#endif //USE_OPENAL_SOUND
	return 0;
}

