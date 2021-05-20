// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  bool zTSoundDescriptor::operator == ( const zTSoundDescriptor& other ) const {
    return Handle == other.Handle;
  }

  float zTSoundDescriptor::GetVolume() {
    int frequency = 0;
    float volume = 0.0f;
    float pan = 0.0f;
    zsound->GetSoundProps( Handle, frequency, volume, pan );
    return volume;
  }

  void zTSoundDescriptor::SetVolume( const float& newVolume ) {
    int frequency = 0;
    float volume = 0.0f;
    float pan = 0.0f;
    zsound->GetSoundProps( Handle, frequency, volume, pan );
    volume = zLimit( newVolume, 0.0f, 0.99f );
    zsound->UpdateSoundProps( Handle, frequency, volume, pan );
  }

  void zTSoundDescriptor::SetLooping( const bool& looping ) {
    zCActiveSnd* snd = zCActiveSnd::GetHandleSound( Handle );
    snd->looping = looping;
  }

  void zTSoundDescriptor::SetPositionMs( const long& position_ms ) {
    if( AIL_set_sample_ms_position ) {
      zCActiveSnd* snd = zCActiveSnd::GetHandleSound( Handle );
      if( snd )
        AIL_set_sample_ms_position( snd->sample, position_ms );
    }
  }

  void zTSoundDescriptor::Pause() {
    if( AIL_stop_sample ) {
      zCActiveSnd* snd = zCActiveSnd::GetHandleSound( Handle );
      if( snd )
        AIL_stop_sample( snd->sample );
    }
  }

  void zTSoundDescriptor::Resume() {
    if( AIL_resume_sample ) {
      zCActiveSnd* snd = zCActiveSnd::GetHandleSound( Handle );
      if( snd ) {
        AIL_resume_sample( snd->sample );
      }
    }
  }
}