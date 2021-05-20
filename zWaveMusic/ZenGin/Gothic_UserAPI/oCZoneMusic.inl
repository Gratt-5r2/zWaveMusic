// Supported with union (c) 2020 Union team

// User API for oCZoneMusic
// Add your methods here

void ProcessZoneListWave( zCArraySort<zCZone*> const&, zCArraySort<zCZone*> const&, zCWorld* );
void ProcessZoneList_Union( zCArraySort<zCZone*> const&, zCArraySort<zCZone*> const&, zCWorld* );
bool IsHighestPriority( zCArraySort<zCZone*> const& zoneList );
bool IsZoneWave();
bool WaitTimeOut();
static string ActiveZoneName;
static uint LastSoundStartTime;
static bool ActiveZoneIsDM();
static bool ActiveZoneIsWave();