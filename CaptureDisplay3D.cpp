//
// Created by lyp on 19.11.20.
//
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include "CaptureDisplay3D.h"

using namespace std;


DeckLinkInputCallback::DeckLinkInputCallback() : m_refCount(0)
{
    pthread_mutex_init(&m_mutex, NULL);
    pthread_mutex_init(&m_mutexGet, NULL);
    this->m_Buffer = 0;
}

DeckLinkInputCallback::~DeckLinkInputCallback()
{
    pthread_mutex_destroy(&m_mutex);
    pthread_mutex_destroy(&m_mutexGet);
}

ULONG DeckLinkInputCallback::AddRef(void)
{
    pthread_mutex_lock(&m_mutex);
    m_refCount++;
    pthread_mutex_unlock(&m_mutex);

    return (ULONG)m_refCount;
}

ULONG DeckLinkInputCallback::Release(void)
{
    pthread_mutex_lock(&m_mutex);
    m_refCount--;
    pthread_mutex_unlock(&m_mutex);

    if (m_refCount == 0) {
        delete this;
        return 0;
    }

    return (ULONG)m_refCount;
}
HRESULT DeckLinkInputCallback::VideoInputFrameArrived(IDeckLinkVideoInputFrame* videoFrame, IDeckLinkAudioInputPacket* audioFrame)
{

}



bool CaptureDisplay3D::InitDecklink() {
    IDeckLinkIterator* deckLinkIterator = NULL;
    IDeckLinkDisplayModeIterator* deckLinkDisplayModeIterator = NULL;
    IDeckLink* deckLink = NULL;
    IDeckLinkDisplayMode* deckLinkDisplayMode = NULL;
    BMDDisplayMode selectedDisplayMode = bmdModeHD1080p50;
    HRESULT	result;
    deckLinkModeIndex = 1;
    selectedDisplayMode = bmdModeHD1080p50;


    deckLinkIterator = CreateDeckLinkIteratorInstance();
    if (deckLinkIterator == NULL){
        printf("DeckLink driver required!");
        return false;
    }

/*    while (deckLinkIterator->Next(&deckLink) == S_OK){
        if (! myDLOutput_left){
            if (deckLink->QueryInterface(IID_IDeckLinkOutput, (void**)&myDLOutput_left) != S_OK){
                myDLOutput_left = NULL;
            }
        }else{
            if (!myDLOutput_right){
                if (deckLink->QueryInterface(IID_IDeckLinkOutput, (void**)&myDLOutput_right) != S_OK){
                    myDLOutput_right = NULL;
                }
            }
        }
        if (myDLOutput_right && myDLOutput_left){
            break;
        }
    }

    if (!myDLOutput_left && !myDLOutput_right){
        printf("two DeckLink devices required!");
        return false;
    }*/
    result = deckLinkIterator->Next(&deckLink);
    if (result != S_OK){
        printf("cannot get decklink device\n");
    }

    result = deckLink->QueryInterface(IID_IDeckLinkInput, (void**)&m_DeckLinkInput);
    if (result !=  S_OK){
        printf("cannot get decklink input\n");
    }

    m_DeckLinkInput->GetDisplayModeIterator(&deckLinkDisplayModeIterator);
    while (deckLinkDisplayModeIterator->Next(&deckLinkDisplayMode) == S_OK)
    {
        displayModes.push_back(deckLinkDisplayMode);
    }

    for (size_t i = 0; i < displayModes.size(); i++)
    {
        char* displayModeName;
        unsigned int supported3D;
        result = displayModes[i]->GetName((const char **)&displayModeName);
        if (result == S_OK)
        {
            BMDTimeValue frameRateDuration;
            BMDTimeValue frameRateScale;

            displayModes[i]->GetFrameRate(&frameRateDuration, &frameRateScale);
            supported3D = displayModes[i]->GetFlags() & bmdDisplayModeSupports3D;
            fprintf(stderr,
                    "        %2d:  %-20s \t %li x %li \t %g FPS \t %du 3D\n",
                    i,
                    displayModeName,
                    displayModes[i]->GetWidth(),
                    displayModes[i]->GetHeight(),
                    (double)frameRateScale / (double)frameRateDuration,
                    supported3D
            );
            free(displayModeName);
        }
    }

    m_DeckLinkInput->GetDisplayMode(selectedDisplayMode, &deckLinkDisplayMode);
    bool displayModeSupported;
    result = m_DeckLinkInput->DoesSupportVideoMode(
            bmdVideoConnectionUnspecified,
            selectedDisplayMode,
//            bmdFormat10BitYUV,
            bmdFormat8BitBGRA,
            bmdNoVideoOutputConversion,
            bmdVideoInputDualStream3D,
            NULL,
            &displayModeSupported);

    if(result != S_OK){
        printf("Could not determine whether video mode is supported");
        return false;
    }
    if (!displayModeSupported)
    {
        fprintf(stderr, "Video mode is not supported\n");
        return false;
    }

    m_DeckLinkInput->EnableVideoInput(selectedDisplayMode, bmdFormat8BitBGRA, bmdVideoInputDualStream3D);
    //m_deckLinkOutput->DoesSupportVideoMode(bmdVideoConnectionUnspecified, displayMode, pixelFormat, bmdNoVideoOutputConversion, supportedVideoModeFlags, nullptr, &displayModeSupported)

}

void CaptureDisplay3D::StartStreaming() {

}