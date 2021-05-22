// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  struct zTSoundDescriptor {
    zCSndFX_MSS* FX;
    int Handle;
    zSTRING Name;
    int Theme;
    float FadeSpeed;
    uint FadeWait;

    zTSoundDescriptor();
    bool operator == ( const zTSoundDescriptor& other ) const;
    float GetVolume();
    void SetVolume( const float& volume );
    void SetLooping( const bool& looping );
    void SetPositionMs( const long& position_ms );
    void Pause();
    void Resume();
    void Unpack();
    void Pack();
  };
}