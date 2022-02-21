#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Resource
 * \File:    le_audio_clip.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_AUDIO_CLIP_H
#define LE_AUDIO_CLIP_H

// LE Includes
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <OpenAL/alut.h>

class LEAudioClip {
private:
    ALuint    m_buffer;
    ALuint    m_source;
    ALenum    m_format;
    ALsizei   m_size;
    ALvoid*   m_data;
    ALsizei   m_freq;
    ALboolean m_loop;

    ALfloat   m_position;
    ALfloat   m_velocity;
public:
	LEAudioClip() {
        alGenBuffers(1, &this->m_buffer);
        alutLoadWAVFile("wavdata/FancyPants.wav", &this->m_format, &this->m_data, &this->m_size, &this->m_freq, &this->m_loop);
        alBufferData(this->m_buffer, this->m_format, this->m_data, this->m_size, this->m_freq);
        alutUnloadWAV(this->m_format, this->m_data, this->m_size, this->m_freq);
        if (alGetError() != AL_NO_ERROR) {
            return;
        }
		alGenSources(1, &this->m_source);
        if (alGetError() != AL_NO_ERROR) {
            return;
        }
        alSourcei(this->m_source,  AL_BUFFER,    this->m_buffer);
        alSourcef(this->m_source,  AL_PITCH,     1.0f);
        alSourcef(this->m_source,  AL_GAIN,      1.0f);
        alSourcefv(this->m_source, AL_POSITION, &this->m_position);
        alSourcefv(this->m_source, AL_VELOCITY, &this->m_velocity);
        alSourcei(this->m_source,  AL_LOOPING,   this->m_loop);
       
        if (alGetError() != AL_NO_ERROR) {
            return;
        }
	}
	virtual ~LEAudioClip() {
        alDeleteBuffers(1, &this->m_buffer);
        alDeleteSources(1, &this->m_source);
	}

};
#endif // !LE_AUDIO_CLIP_H