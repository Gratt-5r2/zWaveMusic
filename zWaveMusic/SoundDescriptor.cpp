// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  inline bool IsBadHandle( const int& handle ) {
    return handle == Invalid || handle == None;
  }

  zTSoundDescriptor::zTSoundDescriptor() {
    FX        = Null;
    Handle    = Invalid;
    Theme     = 0;
    FadeSpeed = 1.0f;
    FadeWait  = 5000;
  }

  bool zTSoundDescriptor::operator == ( const zTSoundDescriptor& other ) const {
    return Name == other.Name;
  }

  float zTSoundDescriptor::GetVolume() {
    if( IsBadHandle( Handle ) )
      return 0.9f;

    int frequency = 0;
    float volume = 0.0f;
    float pan = 0.0f;
    zsound->GetSoundProps( Handle, frequency, volume, pan );
    return volume;
  }

  void zTSoundDescriptor::SetVolume( const float& newVolume ) {
    if( IsBadHandle( Handle ) )
      return;

    int frequency = 0;
    float volume = 0.0f;
    float pan = 0.0f;
    zsound->GetSoundProps( Handle, frequency, volume, pan );
    volume = zLimit( newVolume, 0.0f, 0.99f );
    zsound->UpdateSoundProps( Handle, frequency, volume, pan );
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