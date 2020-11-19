//
// Created by lyp on 19.11.20.
//

#ifndef TEST_DECKLINK_8K_PRO_CAPTUREDISPLAY3D_H
#define TEST_DECKLINK_8K_PRO_CAPTUREDISPLAY3D_H

#include <vector>
#include <pthread.h>

#include "DeckLinkAPI.h"

class DeckLinkInputCallback : public IDeckLinkInputCallback
{
public:
    DeckLinkInputCallback();
    virtual ~DeckLinkInputCallback();
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID /*iid*/, LPVOID */*ppv*/)
    {
        return E_NOINTERFACE;
    }
    virtual ULONG STDMETHODCALLTYPE AddRef(void);
    virtual ULONG STDMETHODCALLTYPE  Release(void);
    virtual HRESULT STDMETHODCALLTYPE VideoInputFormatChanged(BMDVideoInputFormatChangedEvents, IDeckLinkDisplayMode*, BMDDetectedVideoInputFormatFlags);
    virtual HRESULT STDMETHODCALLTYPE VideoInputFrameArrived(IDeckLinkVideoInputFrame*, IDeckLinkAudioInputPacket*);
    bool GetImage(IDeckLinkVideoInputFrame** buffer);
private:
    ULONG m_refCount;
    pthread_mutex_t m_mutex;
    pthread_mutex_t m_mutexGet;
    IDeckLinkVideoInputFrame* m_Buffer;
};


class CaptureDisplay3D {

public:
    bool InitDecklink();
    void StartStreaming();

private:
    IDeckLinkInput* m_DeckLinkInput;
    std::vector<IDeckLinkDisplayMode*> displayModes;
    double deckLinkModeIndex;
    BMDDisplayMode selectedDisplayMode;

};


#endif //TEST_DECKLINK_8K_PRO_CAPTUREDISPLAY3D_H
