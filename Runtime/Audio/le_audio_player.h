#pragma once
#include <cstdint>
#include "Runtime/Resource/le_audio_clip.h"

class LEAudioPlayer {
private:
	LEAudioClip*  m_pAudioClip{nullptr};
	bool          m_silent{false};
	bool          m_playOnBeginning{ false };
	bool          m_looping{ false };
	float         m_volume{ 1.0f};
	float         m_pitch{1.0f};
	uint32_t      m_priority{0};
public:			  
	void          play();
				  
	void          playOnce();
				  
	void          playDelayed(float delayTimeMs);
				  
	void          resume();
				  
	void          stop();
};
