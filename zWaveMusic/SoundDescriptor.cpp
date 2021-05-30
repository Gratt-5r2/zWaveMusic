// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  static float CalcAdaptiveSoundVolume( const float& sourceVolume ) {
    float soundMult = SafeDiv( 1.0f, zCSndSys_MSS::prefs.volume );
    float musicMult = ((zCMusicSys_DirectMusic*)zmusic)->prefs.volume;
    return sourceVolume * soundMult * musicMult;
  }

  inline bool IsBadHandle( const int& handle ) {
    return handle == Invalid || handle == None;
  }

  zTSoundDescriptor::zTSoundDescriptor() {
    FX        = Null;
    Handle    = Invalid;
    Theme     = 0;
    FadeSpeed = 1.0f;
    Volume    = 1.0f;
    FadeWait  = 5000;
  }

  bool zTSoundDescriptor::operator == ( const zTSoundDescriptor& other ) const {
    return Name == other.Name;
  }

  float zTSoundDescriptor::GetVolume() {
    return Volume;
  }

  float zTSoundDescriptor::GetVolumeAdaptive() {
    return CalcAdaptiveSoundVolume( Volume );
  }

  void zTSoundDescriptor::SetVolume( const float& newVolume ) {
    if( IsBadHandle( Handle ) )
      return;

    Volume = newVolume;
    UpdateVolume();
  }

  void zTSoundDescriptor::UpdateVolume() {
    if( IsBadHandle( Handle ) )
      return;

    float volumeAdaptive = GetVolumeAdaptive();

    if( AIL_set_sample_volume_pan ) {
      zCActiveSnd* snd = zCActiveSnd::GetHandleSound( Handle );
      if( snd )
        AIL_set_sample_volume_pan( snd->sample, volumeAdaptive, 0.5f );
    }
  }

  void zTSoundDescriptor::SetLooping( const bool& looping ) {
    if( IsBadHandle( Handle ) )
      return;

    zCActiveSnd* snd = zCActiveSnd::GetHandleSound( Handle );
    snd->looping = looping;
  }

  void zTSoundDescriptor::SetPositionMs( const long& position_ms ) {
    if( IsBadHandle( Handle ) )
      return;

    if( AIL_set_sample_ms_position ) {
      zCActiveSnd* snd = zCActiveSnd::GetHandleSound( Handle );
      if( snd )
        AIL_set_sample_ms_position( snd->sample, position_ms );
    }
  }

  void zTSoundDescriptor::Pause() {
    if( IsBadHandle( Handle ) )
      return;

    if( AIL_stop_sample ) {
      zCActiveSnd* snd = zCActiveSnd::GetHandleSound( Handle );
      if( snd )
        AIL_stop_sample( snd->sample );
    }
  }

  void zTSoundDescriptor::Resume() {
    if( IsBadHandle( Handle ) )
      return;

    if( AIL_resume_sample ) {
      zCActiveSnd* snd = zCActiveSnd::GetHandleSound( Handle );
      if( snd ) {
        AIL_resume_sample( snd->sample );
      }
    }
  }

  void zTSoundDescriptor::Unpack() {
    vdf_unpackogg( Name.ToChar(), VDF_DEFAULT );
  }

  void zTSoundDescriptor::Pack() {
    vdf_packogg( Name.ToChar(), VDF_DEFAULT );
  }
}