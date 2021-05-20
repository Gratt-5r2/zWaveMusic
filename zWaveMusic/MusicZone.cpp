// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  string oCZoneMusic::ActiveZoneName = "";
  uint oCZoneMusic::LastSoundStartTime = 0;



  HOOK Hook_oCZoneMusic_ProcessZoneList PATCH( &oCZoneMusic::ProcessZoneList, &oCZoneMusic::ProcessZoneList_Union );

  void oCZoneMusic::ProcessZoneList_Union( zCArraySort<zCZone*> const& zoneList, zCArraySort<zCZone*> const& zoneDeactivateList, zCWorld* world ) {
    static Timer timer;
    if( WaitTimeOut() )
      return;

    if( !zmusic || zmusic->s_musicSystemDisabled || !IsHighestPriority( zoneList ) )
      return;

    if( IsZoneWave() ) {
      timer[0u].Delete();
      return ProcessZoneListWave( zoneList, zoneDeactivateList, world );
    }

    bool waveToDm = ActiveZoneIsWave();
    if( waveToDm ) {
      if( oCSoundController::GetInstance().SoundIsActive() ) {
        oCSoundController::GetInstance().PlaySoundOut( "" );
        oCSoundController::GetInstance().StopSound();
      }

      if( !timer[0u].Await( SOUND_START_DELAY ) )
        return;

      LastSoundStartTime = Timer::GetTime();
    }

    ActiveZoneName = (A objectName).Upper();
    THISCALL( Hook_oCZoneMusic_ProcessZoneList )(zoneList, zoneDeactivateList, world);
  }



  void oCZoneMusic::ProcessZoneListWave( zCArraySort<zCZone*> const& zoneList, zCArraySort<zCZone*> const& zoneDeactivateList, zCWorld* world ) {
    oCSoundController& sndCtrl = oCSoundController::GetInstance();
    string SoundName = GetObjectName();
    if( sndCtrl.SoundIsActive() && SoundName == sndCtrl.GetSoundName() )
      return;

    bool dmToWav = ActiveZoneIsDM();
    if( dmToWav ) {
      zmusic->PlayThemeByScript( "SYS_LOADING", 0, 0 );
      zCSoundFX* sound = zsound->LoadSoundFX( "DEFAULT_FX_OUT.WAV" );
      if( sound )
        zsound->PlaySound( sound, SOUND_THEME_CHANNEL_OUT );
    }
    else
      zmusic->Stop();

    if( s_musiczone != this ) {
      s_oldmusiczone = s_musiczone;
      s_musiczone = this;
    }

    ActiveZoneName = (A objectName).Upper();
    oCSoundController::GetInstance().PlaySound( SoundName, dmToWav );
    LastSoundStartTime = Timer::GetTime();
  }



  bool oCZoneMusic::IsHighestPriority( zCArraySort<zCZone*> const& zoneList ) {
    if( zoneList.GetNum() <= 1 )
      return true;

    for( int i = 0; i < zoneList.GetNum(); i++ ) {
      oCZoneMusic* musicZone = zoneList[i]->CastTo<oCZoneMusic>();
      if( !musicZone || this == musicZone )
        continue;

      if( priority <= musicZone->priority )
        return false;
    }

    return true;
  }



  bool oCZoneMusic::IsZoneWave() {
    string themeName = objectName;
    themeName.Upper();
    return themeName.EndWith( ".WAV" );
  }



  bool oCZoneMusic::WaitTimeOut() {
    uint now = Timer::GetTime();
    if( now - LastSoundStartTime < SOUND_INTERVAL_BETWEEN_THEMES )
      return true;

    return false;
  }



  bool oCZoneMusic::ActiveZoneIsDM() {
    return !ActiveZoneName.IsEmpty() && !ActiveZoneName.EndWith( ".WAV" );
  }



  bool oCZoneMusic::ActiveZoneIsWave() {
    return !ActiveZoneName.IsEmpty() && ActiveZoneName.EndWith( ".WAV" );
  }
}