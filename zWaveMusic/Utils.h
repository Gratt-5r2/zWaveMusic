// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  template <class T>
  inline T zLimit( const T& value, const T& min, const T& max ) {
    if( value < min ) return min;
    if( value > max ) return max;
    return value;
  }

  void* GetMssAddress( const string& name ) {
    HMODULE module = GetModuleHandle( "Mss32.dll" );
    if( !module )
      return Null;

    return GetProcAddress( module, name );
  }

  typedef void( __stdcall* AILSAMPLECB )(void* S);
  typedef void( __stdcall* LPAIL_SET_SAMPLE_MS_POSITION )(void* S, long milliseconds);
  typedef void( __stdcall* LPAIL_STOP_SAMPLE )(void* S);
  typedef void( __stdcall* LPAIL_RESUME_SAMPLE )(void* S);
  typedef void( __stdcall* LPAIL_SET_SAMPLE_LOOP_COUNT )(void* S, long loop_count);
  typedef void( __stdcall* LPAIL_REGISTER_EOF_CALLBACK )(void* S, AILSAMPLECB callback);
  typedef void( __stdcall* LPAIL_SET_SAMPLE_VOLUME_LEVELS )(void* S, float left_volume, float right_volume);
  typedef void( __stdcall* LPAIL_SET_SAMPLE_VOLUME_PAN )(void* S, float volume, float pan);

  static auto AIL_set_sample_ms_position   = (LPAIL_SET_SAMPLE_MS_POSITION)  GetMssAddress( "_AIL_set_sample_ms_position@8" );
  static auto AIL_stop_sample              = (LPAIL_STOP_SAMPLE)             GetMssAddress( "_AIL_stop_sample@4" );
  static auto AIL_resume_sample            = (LPAIL_RESUME_SAMPLE)           GetMssAddress( "_AIL_resume_sample@4" );
  static auto AIL_set_sample_loop_count    = (LPAIL_SET_SAMPLE_LOOP_COUNT)   GetMssAddress( "_AIL_set_sample_loop_count@8" );
  static auto AIL_register_EOF_callback    = (LPAIL_REGISTER_EOF_CALLBACK)   GetMssAddress( "_AIL_register_EOF_callback@8" );
  static auto AIL_set_sample_volume_levels = (LPAIL_SET_SAMPLE_VOLUME_LEVELS)GetMssAddress( "_AIL_set_sample_volume_levels@12" );
  static auto AIL_set_sample_volume_pan    = (LPAIL_SET_SAMPLE_VOLUME_LEVELS)GetMssAddress( "_AIL_set_sample_volume_pan@12" );
}