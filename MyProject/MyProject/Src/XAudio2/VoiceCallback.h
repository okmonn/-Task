#pragma once
#include <xaudio2.h>

class VoiceCallback :
	public IXAudio2VoiceCallback
{
public:
	// コンストラクタ
	VoiceCallback() : handle(CreateEvent(nullptr, false, false, nullptr)) {
	}
	// デストラクタ
	~VoiceCallback() {
		CloseHandle(handle);
	}

	void STDMETHODCALLTYPE OnStreamEnd() {
	}
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() {
	}
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired) {
	}
	void STDMETHODCALLTYPE OnBufferEnd(void * pBufferContext) {
		SetEvent(handle);
	}
	void STDMETHODCALLTYPE OnBufferStart(void * pBufferContext) {
	}
	void STDMETHODCALLTYPE OnLoopEnd(void * pBufferContext) {
	}
	void STDMETHODCALLTYPE OnVoiceError(void * pBufferContext, HRESULT Error) {
	}

	// ハンドル
	HANDLE handle;

private:

};
