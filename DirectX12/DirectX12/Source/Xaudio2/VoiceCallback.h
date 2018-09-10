#pragma once
#include <xaudio2.h>
#include <iostream>

class VoiceCallback : 
	public IXAudio2VoiceCallback
{
public:
	// コンストラクタ
	VoiceCallback() {
		handle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	}
	// デストラクタ
	~VoiceCallback() {
		CloseHandle(handle);
	}

	void STDMETHODCALLTYPE OnStreamEnd() { 
		//printf("%s\n", __func__); 
	}
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() {
		//printf("%s\n", __func__);
	}
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired) {
		//printf("%s\n", __func__);
	}
	void STDMETHODCALLTYPE OnBufferEnd(void * pBufferContext) { 
		SetEvent(handle); 
		//printf("%s\n", __func__);
	}
	void STDMETHODCALLTYPE OnBufferStart(void * pBufferContext) {
		//printf("%s\n", __func__);
	}
	void STDMETHODCALLTYPE OnLoopEnd(void * pBufferContext) { 
		//printf("%s\n", __func__); 
	}
	void STDMETHODCALLTYPE OnVoiceError(void * pBufferContext, HRESULT Error) { 
		//printf("%s\n", __func__); 
	}

	// ハンドル
	HANDLE handle;

private:

};

