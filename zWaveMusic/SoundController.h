// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
#define SOUND_START_DELAY             (2800)
#define SOUND_THEME_CHANNEL           (9999)
#define SOUND_THEME_CHANNEL_OUT       (10000)
#define SOUND_INTERVAL_BETWEEN_THEMES (5000)

  enum zESoundTheme {
    THEME_DEFAULT,
    THEME_NIGHT,
    THEME_THREAT,
    THEME_FIGHT,
    THEME_MAX
  };

  class oCSoundController {
    static Array<zTSoundDescriptor> FadeIn;
    static Array<zTSoundDescriptor> FadeOut;
    void SetFadeIn( zTSoundDescriptor sound, const uint& delay = 0, const float& fadeSpeed = 0.05f );
    void SetFadeOut( zTSoundDescriptor sound, const uint& delay = 0, const float& fadeSpeed = 0.05f );

    zTSoundDescriptor Sound;
    zESoundTheme Theme;
    zSTRING SoundName;
    uint PlayStartTime;

    zSTRING GetSoundThemeName( const zSTRING& nextSoundName, const zSTRING& themeName );
    zSTRING GetSoundTransferName( const zSTRING& nextSoundName, const zSTRING& themeName );
    void SetTheme( const zESoundTheme& theme );
    void PlaySoundTransfer( const zSTRING& soundName );
    void FadeLoopIn();
    void FadeLoopOut();
    oCSoundController();
  public:

    zSTRING GetSoundNameByTheme();
    bool StartTimeIsPassed();
    bool SoundIsActive();
    void PlaySound( const zSTRING& soundName, const bool& forciblyDelay = false );
    void StopSound();
    void PlaySoundOut( const zSTRING& nextSoundName );
    void EmergencyStopSound();
    void FadeLoop();
    void UpdateTheme();
    string GetSoundName();
    string GetSoundNameActive();
    static oCSoundController& GetInstance();
    static bool SoundIsExists( const zSTRING& soundName );
  };
}