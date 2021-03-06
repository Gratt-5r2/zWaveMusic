// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  Array<zTSoundDescriptor> oCSoundController::FadeIn;
  Array<zTSoundDescriptor> oCSoundController::FadeOut;


  zCSndSys_MSS* GetMusicMss() {
    // static zCSndSys_MSS* zsoundMss = new zCSndSys_MSS();
    // return zsoundMss;
    return (zCSndSys_MSS*)zsound;
  }

  void zDieter_StartUp( HWND* initContextHandle );
  HOOK Hook_zDieter_StartUp PATCH_IF( &zDieter_StartUp, &zDieter_StartUp, false );
  void zDieter_StartUp( HWND* initContextHandle ) {
    cmd << "init mss" << endl;
    GetMusicMss();
    Hook_zDieter_StartUp( initContextHandle );
  }



  oCSoundController::oCSoundController() {
    Sound.FX      = Null;
    Sound.Handle  = Invalid;
    Sound.Name    = "";
    SoundName     = "";
    PlayStartTime = 0;
    Theme         = THEME_DEFAULT;
    UpdateTheme();
  }



  zSTRING oCSoundController::GetSoundNameByTheme() {
    if( Theme == THEME_NIGHT ) {
      zSTRING soundName = GetSoundThemeName( SoundName, "NIGHT" );
      if( !soundName.IsEmpty() )
        return soundName;
    }
    else if( Theme == THEME_THREAT ) {
      zSTRING soundName = GetSoundThemeName( SoundName, "THREAT" );
      if( !soundName.IsEmpty() )
        return soundName;
    }
    else if( Theme == THEME_FIGHT ) {
      zSTRING soundName = GetSoundThemeName( SoundName, "FIGHT" );
      if( !soundName.IsEmpty() )
        return soundName;
    }

    return SoundName;
  }



  bool oCSoundController::StartTimeIsPassed() {
    return Timer::GetTime() - PlayStartTime > SOUND_INTERVAL_BETWEEN_THEMES;
  }



  bool oCSoundController::SoundIsActive() {
    return Sound.FX != Null;
  }



  void oCSoundController::PlaySound( const zSTRING& soundName, const bool& forciblyDelay ) {
    if( soundName.IsEmpty() || (SoundIsActive() && soundName == SoundName) )
      return;

    if( soundName == Sound.Name ) {
      SoundName = soundName;
      return;
    }

    bool delayed = (Sound.FX ? true : false) || forciblyDelay;

    if( SoundIsActive() ) {
      PlaySoundOut( soundName );
      StopSound();
    }

    SoundName = soundName;
    Sound.Name = GetSoundNameByTheme();
    Sound.FX = (zCSndFX_MSS*)GetMusicMss()->LoadSoundFX( Sound.Name );
    if( !Sound.FX )
      return;

    if( !delayed ) {
      Sound.Handle = GetMusicMss()->PlaySound( Sound.FX, SOUND_THEME_CHANNEL );
      Sound.FX->Release();
      Sound.SetVolume( 1.0f );
      Sound.SetLooping( True );
    }
    else {
      Sound.Unpack();
      Sound.Handle = Invalid;
    }

    PlayStartTime = Timer::GetTime();
    uint delay = delayed ? SOUND_START_DELAY : 0;
    SetFadeIn( Sound, delay, 1.0f );
  }



  void oCSoundController::PlaySoundTransfer( const zSTRING& soundName ) {
    if( soundName.IsEmpty() || soundName == Sound.Name )
      return;

    bool delayed = Sound.FX ? true : false;

    if( SoundIsActive() ) {
      PlaySoundOut( Sound.Name );
      StopSound();
    }

    Sound.Name = soundName;
    Sound.FX = (zCSndFX_MSS*)GetMusicMss()->LoadSoundFX( Sound.Name );
    if( !Sound.FX )
      return;

    if( !delayed ) {
      Sound.Handle = GetMusicMss()->PlaySound( Sound.FX, SOUND_THEME_CHANNEL );
      Sound.FX->Release();
      Sound.SetVolume( 1.0f );
      Sound.SetLooping( True );
    }
    else {
      Sound.Unpack();
      Sound.Handle = Invalid;
    }

    PlayStartTime = Timer::GetTime();
    uint delay = delayed ? SOUND_START_DELAY : 0;
    SetFadeIn( Sound, delay, 1.0f );
  }



  void oCSoundController::StopSound() {
    SetFadeOut( Sound, 0 );
    Sound.FX     = Null;
    Sound.Handle = Invalid;
    Sound.Name   = "";
  }



  void oCSoundController::EmergencyStopSound() {
    if( Sound.FX )
      Sound.FX->CacheOut();

    for( uint i = 0; i < FadeIn.GetNum(); i++ )
      FadeIn[i].Pack();

    for( uint i = 0; i < FadeOut.GetNum(); i++ )
      FadeOut[i].Pack();

    FadeIn.Clear();
    FadeOut.Clear();

    Sound.FX     = Null;
    Sound.Handle = Invalid;
    Sound.Name   = "";
    SoundName    = "";
  }



  zSTRING oCSoundController::GetSoundThemeName( const zSTRING& nextSoundName, const zSTRING& themeName ) {
    zSTRING soundDefault = string::Combine( "%z_%z.WAV", SoundName.GetWord( "." ), themeName );
    if( vdf_fexists( soundDefault.ToChar(), VDF_DEFAULT ) )
      return soundDefault;

    return "";
  }



  zSTRING oCSoundController::GetSoundTransferName( const zSTRING& nextSoundName, const zSTRING& themeName ) {
    zSTRING soundSpecific = string::Combine( "%z_%z_%z.WAV", Sound.Name.GetWord( "." ), themeName, nextSoundName.GetWord( "." ) );
    if( vdf_fexists( soundSpecific.ToChar(), VDF_DEFAULT ) )
      return soundSpecific;

    zSTRING soundDefault = string::Combine( "%z_%z.WAV", Sound.Name.GetWord( "." ), themeName );
    if( vdf_fexists( soundDefault.ToChar(), VDF_DEFAULT ) )
      return soundDefault;

    return "";
  }



  void oCSoundController::PlaySoundOut( const zSTRING& nextSoundName ) {
    zSTRING soundName = GetSoundTransferName( nextSoundName, "OUT" );
    if( soundName.IsEmpty() ) {
      soundName = GetSoundThemeName( SoundName, "OUT" );
      if( soundName.IsEmpty() )
        return;
    }

    zCSndFX_MSS* soundFX = (zCSndFX_MSS*)GetMusicMss()->LoadSoundFX( soundName );
    if( !soundFX )
      return;

    zTSoundDescriptor Sound;
    Sound.FX     = soundFX;
    Sound.Handle = GetMusicMss()->PlaySound( soundFX, SOUND_THEME_CHANNEL_OUT );
    Sound.SetVolume( 1.0f );
    soundFX->Release();
  }



  oCSoundController& oCSoundController::GetInstance() {
    static oCSoundController controller;
    return controller;
  }



  void oCSoundController::FadeLoopIn() {
    for( uint i = 0; i < FadeIn.GetNum(); i++ ) {
      zTSoundDescriptor& sound = FadeIn[i];
      if( sound.FadeWait )
        continue;

      float volume = sound.GetVolume();
      if( volume < 1.0f ) {
        sound.SetVolume( volume + sound.FadeSpeed );
        continue;
      }

      FadeIn.RemoveAt( i-- );
    }
  }



  void oCSoundController::FadeLoopOut() {
    for( uint i = 0; i < FadeOut.GetNum(); i++ ) {
      zTSoundDescriptor& sound = FadeOut[i];
      if( sound.FadeWait )
        continue;

      float volume = sound.GetVolume();
      if( volume > 0.01f ) {
        sound.SetVolume( volume - sound.FadeSpeed );
        break;
      }
      
      if( !IsBadHandle( sound.Handle ) )
        GetMusicMss()->StopSound( sound.Handle );

      if( sound.FX ) {
        sound.FX->CacheOut();
        sound.FX->Release();
        sound.FX = Null;
      }

      FadeOut.RemoveAt( i-- );
    }
  }



  void oCSoundController::FadeLoop() {
    static Timer timer;
    timer.ClearUnused();

    for( uint i = 0; i < FadeIn.GetNum(); i++ ) {
      zTSoundDescriptor& sound = FadeIn[i];
      if( sound.FadeWait ) {
        if( timer[(uint)sound.FX].Await( sound.FadeWait ) ) {
          if( IsBadHandle( sound.Handle ) ) {
            Sound.Handle = GetMusicMss()->PlaySound( sound.FX, SOUND_THEME_CHANNEL );
            sound.FX->Release();
            sound.Handle = Sound.Handle;
            sound.Pack();
            Sound.SetVolume( 1.0f );
            Sound.SetLooping( True );
          }

          sound.FadeWait = 0;
          sound.SetPositionMs( 0 );
          sound.Resume();
          zmusic->Stop();
        }
        else {
          sound.Pause();
          continue;
        }
      }
    }

    if( timer[0u].Await( 100 ) )
      FadeLoopIn();

    if( timer[1u].Await( 100 ) )
      FadeLoopOut();
  }



  void oCSoundController::SetFadeIn( zTSoundDescriptor sound, const uint& delay, const float& fadeSpeed ) {
    sound.FadeSpeed = fadeSpeed;
    sound.FadeWait = delay;
    FadeOut ^= sound;
    FadeIn |= sound;
  }



  void oCSoundController::SetFadeOut( zTSoundDescriptor sound, const uint& delay, const float& fadeSpeed ) {
    if( sound.FX == Null )
      return;

    sound.FadeSpeed = fadeSpeed;
    sound.FadeWait = delay;
    FadeIn ^= sound;
    FadeOut |= sound;
  }



  void oCSoundController::UpdateTheme() {
    if( ogame->IsOnPause() && SoundIsActive() )
      Sound.UpdateVolume();

    if( !StartTimeIsPassed() )
      return;

    oHEROSTATUS status = ogame->GetHeroStatus();

    switch( status ) 	{
      case Gothic_II_Addon::oHERO_STATUS_STD:
      {
        bool_t isNight = ogame->GetWorldTimer()->IsNight();
             if(  isNight && Theme != THEME_NIGHT   ) SetTheme( THEME_NIGHT   );
        else if( !isNight && Theme != THEME_DEFAULT ) SetTheme( THEME_DEFAULT );
        break;
      }

      case Gothic_II_Addon::oHERO_STATUS_THR:
        if( Theme != THEME_THREAT )
          SetTheme( THEME_THREAT );
        break;

      case Gothic_II_Addon::oHERO_STATUS_FGT:
        if( Theme != THEME_FIGHT )
          SetTheme( THEME_FIGHT );
    }
  }



  void oCSoundController::SetTheme( const zESoundTheme& theme ) {
    if( Theme == theme )
      return;

    if( Sound.Name.IsEmpty() )
      return;

    Theme = theme;
    zSTRING soundName = SoundName;

    switch( Theme ) 	{
      case Gothic_II_Addon::THEME_NIGHT:
        soundName = GetSoundThemeName( SoundName, "NIGHT" );
        break;
      case Gothic_II_Addon::THEME_THREAT:
        soundName = GetSoundThemeName( SoundName, "THREAT" );
        break;
      case Gothic_II_Addon::THEME_FIGHT:
        soundName = GetSoundThemeName( SoundName, "FIGHT" );
        break;
    }

    if( !soundName.IsEmpty() )
      PlaySoundTransfer( soundName );
  }



  bool oCSoundController::SoundIsExists( const zSTRING& soundName ) {
    return 0;
  }



  string oCSoundController::GetSoundName() {
    return SoundName;
  }



  string oCSoundController::GetSoundNameActive() {
    return Sound.Name;
  }
}