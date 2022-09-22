/****************************************************************
 * Class - AudioHandler
 * 
   * This class wraps the different audio output options for 
   * pinball machines (WAV Trigger, SB-100, SB-300, Squawk & Talk
   * -51, etc.) to provide different output options. Additionally,
   * it adds a bunch of audio management features:
   * 
   *   1) Different volume controls for FX, callouts, and music
   *   2) Automatically ducks music behind callouts
   *   3) Supports background soundtracks or looping songs
   *   4) A sound can be queued to play at a future time
   *   5) Callouts can be given different priorities
   *   6) Queued callouts at the same priority will be stacked
 *   
 *   
 * Typical usage:
 *   A global variable of class "AudioHandler" is declared
 *     (ex: "AudioHandler Audio;")
 * 
 *   During the setup() function:
 *    Audio.InitDevices(AUDIO_PLAY_TYPE_WAV_TRIGGER | AUDIO_PLAY_TYPE_ORIGINAL_SOUNDS); // declare what type of audio should be supported
 *    Audio.StopAllAudio(); // Stop any currently playing sounds 
 *    Audio.SetMusicDuckingGain(12); // negative gain applied to music when callouts are being played
 *    Audio.QueueSound(SOUND_EFFECT_MACHINE_INTRO, AUDIO_PLAY_TYPE_WAV_TRIGGER, CurrentTime+1200); // Play machine startup sound in 1.2 s
 *   
 *   Volumes set (pulled from EEPROM or set in setup routine):
 *    Audio.SetMusicVolume(MusicVolume); // value from 0-10 (inclusive)
 *    Audio.SetSoundFXVolume(SoundEffectsVolume); // value from 0-10 (inclusive)
 *    Audio.SetNotificationsVolume(CalloutsVolume); // value from 0-10 (inclusive)
 *   
 *   During the loop():
 *    Audio.Update(CurrentTime);
 *   
 *   During game play:
 *    Audio.PlayBackgroundSong(songNum, true); // loop a background song
 *    Audio.PlaySound(soundEffectNum, AUDIO_PLAY_TYPE_WAV_TRIGGER); // play sound effect through wav trigger
 *    Audio.QueueSound(0x02, AUDIO_PLAY_TYPE_ORIGINAL_SOUNDS, CurrentTime); // Queue sound card command for now
 *    Audio.QueueNotification(soundEffectNum, VoiceNotificationDurations[soundEffectNum-SOUND_EFFECT_VP_VOICE_NOTIFICATIONS_START], priority, CurrentTime); // Queue notification
 *    
 *   End of ball:
 *    Audio.StopAllAudio(); // Stop audio
 */


#include <Arduino.h>
#include "AudioHandler.h"

void SendOnlyWavTrigger::start(void) {
//  uint8_t txbuf[5];
	WTSerial.begin(57600);
}


// **************************************************************
/*
void SendOnlyWavTrigger::masterGain(int gain) {

uint8_t txbuf[7];
unsigned short vol;

	txbuf[0] = SOM1;
	txbuf[1] = SOM2;
	txbuf[2] = 0x07;
	txbuf[3] = CMD_MASTER_VOLUME;
	vol = (unsigned short)gain;
	txbuf[4] = (uint8_t)vol;
	txbuf[5] = (uint8_t)(vol >> 8);
	txbuf[6] = EOM;
	WTSerial.write(txbuf, 7);
}
*/
// **************************************************************
/*
void SendOnlyWavTrigger::setAmpPwr(bool enable) {

uint8_t txbuf[6];

    txbuf[0] = SOM1;
    txbuf[1] = SOM2;
    txbuf[2] = 0x06;
    txbuf[3] = CMD_AMP_POWER;
    txbuf[4] = enable;
    txbuf[5] = EOM;
    WTSerial.write(txbuf, 6);
}
*/

// **************************************************************
/*
void SendOnlyWavTrigger::setReporting(bool enable) {

uint8_t txbuf[6];

	txbuf[0] = SOM1;
	txbuf[1] = SOM2;
	txbuf[2] = 0x06;
	txbuf[3] = CMD_SET_REPORTING;
	txbuf[4] = enable;
	txbuf[5] = EOM;
	WTSerial.write(txbuf, 6);
}
*/

// **************************************************************
void SendOnlyWavTrigger::trackPlaySolo(int trk) {
  
	trackControl(trk, TRK_PLAY_SOLO);
}

// **************************************************************
void SendOnlyWavTrigger::trackPlaySolo(int trk, bool lock) {
  
	trackControl(trk, TRK_PLAY_SOLO, lock);
}

// **************************************************************
void SendOnlyWavTrigger::trackPlayPoly(int trk) {
  
	trackControl(trk, TRK_PLAY_POLY);
}

// **************************************************************
void SendOnlyWavTrigger::trackPlayPoly(int trk, bool lock) {
  
	trackControl(trk, TRK_PLAY_POLY, lock);
}

// **************************************************************
void SendOnlyWavTrigger::trackLoad(int trk) {
  
	trackControl(trk, TRK_LOAD);
}

// **************************************************************
void SendOnlyWavTrigger::trackLoad(int trk, bool lock) {
  
	trackControl(trk, TRK_LOAD, lock);
}

// **************************************************************
void SendOnlyWavTrigger::trackStop(int trk) {

	trackControl(trk, TRK_STOP);
}

// **************************************************************
void SendOnlyWavTrigger::trackPause(int trk) {

	trackControl(trk, TRK_PAUSE);
}

// **************************************************************
void SendOnlyWavTrigger::trackResume(int trk) {

	trackControl(trk, TRK_RESUME);
}

// **************************************************************
void SendOnlyWavTrigger::trackLoop(int trk, bool enable) {
 
	if (enable)
		trackControl(trk, TRK_LOOP_ON);
	else
		trackControl(trk, TRK_LOOP_OFF);
}

// **************************************************************
void SendOnlyWavTrigger::trackControl(int trk, int code) {
  
uint8_t txbuf[8];

	txbuf[0] = SOM1;
	txbuf[1] = SOM2;
	txbuf[2] = 0x08;
	txbuf[3] = CMD_TRACK_CONTROL;
	txbuf[4] = (uint8_t)code;
	txbuf[5] = (uint8_t)trk;
	txbuf[6] = (uint8_t)(trk >> 8);
	txbuf[7] = EOM;
	WTSerial.write(txbuf, 8);
}

// **************************************************************
void SendOnlyWavTrigger::trackControl(int trk, int code, bool lock) {
  
uint8_t txbuf[9];

	txbuf[0] = SOM1;
	txbuf[1] = SOM2;
	txbuf[2] = 0x09;
	txbuf[3] = CMD_TRACK_CONTROL_EX;
	txbuf[4] = (uint8_t)code;
	txbuf[5] = (uint8_t)trk;
	txbuf[6] = (uint8_t)(trk >> 8);
	txbuf[7] = lock;
	txbuf[8] = EOM;
	WTSerial.write(txbuf, 9);
}

// **************************************************************
void SendOnlyWavTrigger::stopAllTracks(void) {

uint8_t txbuf[5];

	txbuf[0] = SOM1;
	txbuf[1] = SOM2;
	txbuf[2] = 0x05;
	txbuf[3] = CMD_STOP_ALL;
	txbuf[4] = EOM;
	WTSerial.write(txbuf, 5);
}

// **************************************************************
void SendOnlyWavTrigger::resumeAllInSync(void) {

uint8_t txbuf[5];

	txbuf[0] = SOM1;
	txbuf[1] = SOM2;
	txbuf[2] = 0x05;
	txbuf[3] = CMD_RESUME_ALL_SYNC;
	txbuf[4] = EOM;
	WTSerial.write(txbuf, 5);
}

// **************************************************************
void SendOnlyWavTrigger::trackGain(int trk, int gain) {

uint8_t txbuf[9];
unsigned short vol;

	txbuf[0] = SOM1;
	txbuf[1] = SOM2;
	txbuf[2] = 0x09;
	txbuf[3] = CMD_TRACK_VOLUME;
	txbuf[4] = (uint8_t)trk;
	txbuf[5] = (uint8_t)(trk >> 8);
	vol = (unsigned short)gain;
	txbuf[6] = (uint8_t)vol;
	txbuf[7] = (uint8_t)(vol >> 8);
	txbuf[8] = EOM;
	WTSerial.write(txbuf, 9);
}

// **************************************************************
void SendOnlyWavTrigger::trackFade(int trk, int gain, int time, bool stopFlag) {

uint8_t txbuf[12];
unsigned short vol;

	txbuf[0] = SOM1;
	txbuf[1] = SOM2;
	txbuf[2] = 0x0c;
	txbuf[3] = CMD_TRACK_FADE;
	txbuf[4] = (uint8_t)trk;
	txbuf[5] = (uint8_t)(trk >> 8);
	vol = (unsigned short)gain;
	txbuf[6] = (uint8_t)vol;
	txbuf[7] = (uint8_t)(vol >> 8);
	txbuf[8] = (uint8_t)time;
	txbuf[9] = (uint8_t)(time >> 8);
	txbuf[10] = stopFlag;
	txbuf[11] = EOM;
	WTSerial.write(txbuf, 12);
}

// **************************************************************
/*
void SendOnlyWavTrigger::samplerateOffset(int offset) {

uint8_t txbuf[7];
unsigned short off;

	txbuf[0] = SOM1;
	txbuf[1] = SOM2;
	txbuf[2] = 0x07;
	txbuf[3] = CMD_SAMPLERATE_OFFSET;
	off = (unsigned short)offset;
	txbuf[4] = (uint8_t)off;
	txbuf[5] = (uint8_t)(off >> 8);
	txbuf[6] = EOM;
	WTSerial.write(txbuf, 7);
}
*/





AudioHandler::AudioHandler() {
  curSoundtrack = NULL;
  curSoundtrackEntries = 0;
  soundFXGain = 0;
  notificationsGain = 0;
  musicGain = 0;
  ClearSoundQueue();
  ClearSoundCardQueue();
  currentBackgroundTrack = BACKGROUND_TRACK_NONE;
  soundtrackRandomOrder = true;
  nextSoundtrackPlayTime = 0;
  backgroundSongEndTime = 0;
  
  voiceNotificationStackFirst = 0;
  voiceNotificationStackLast = 0;
  currentNotificationPriority = 0;
  currentNotificationPlaying = 0;
  ducking = 20;

  for (int count=0; count<NUMBER_OF_SONGS_REMEMBERED; count++) lastSongsPlayed[count] = BACKGROUND_TRACK_NONE;


}

AudioHandler::~AudioHandler() {
}


boolean AudioHandler::InitDevices(byte audioType) {


#if defined(USE_WAV_TRIGGER) || defined(USE_WAV_TRIGGER_1p3)
  if (audioType & AUDIO_PLAY_TYPE_WAV_TRIGGER) {
    // WAV Trigger startup at 57600
    wTrig.start();
    wTrig.stopAllTracks();
  }
#endif

  if (audioType & AUDIO_PLAY_TYPE_ORIGINAL_SOUNDS) {
#ifdef BALLY_STERN_OS_USE_SB300
    InitSB300Registers();
    PlaySB300StartupBeep();
#endif
  }

  return true;
}



int AudioHandler::ConvertVolumeSettingToGain(byte volumeSetting) {
  if (volumeSetting==0) return -70;
  if (volumeSetting>10) return 0;
  return volumeToGainConversion[volumeSetting];
}


void AudioHandler::SetSoundFXVolume(byte s_volume) {
  soundFXGain = ConvertVolumeSettingToGain(s_volume);
}

void AudioHandler::SetNotificationsVolume(byte s_volume) {
  notificationsGain = ConvertVolumeSettingToGain(s_volume);;
}

void AudioHandler::SetMusicVolume(byte s_volume) {
  musicGain = ConvertVolumeSettingToGain(s_volume);;
}

void AudioHandler::SetMusicDuckingGain(byte s_ducking) {
  ducking = s_ducking;
}



boolean AudioHandler::StopSound(unsigned short soundIndex) {
#if defined(USE_WAV_TRIGGER) || defined(USE_WAV_TRIGGER_1p3)
  wTrig.trackStop(soundIndex);
#endif
  return false;
}

boolean AudioHandler::StopAllMusic() {
#if defined(USE_WAV_TRIGGER) || defined(USE_WAV_TRIGGER_1p3)
  if (currentBackgroundTrack!=BACKGROUND_TRACK_NONE) {
    wTrig.trackStop(currentBackgroundTrack);
    currentBackgroundTrack = BACKGROUND_TRACK_NONE;
    return true;
  }
#endif
  currentBackgroundTrack = BACKGROUND_TRACK_NONE;
  curSoundtrack = NULL;
  return false;
}


void AudioHandler::ClearNotificationStack() {
  voiceNotificationStackFirst = 0;
  voiceNotificationStackLast = 0;
}


boolean AudioHandler::StopCurrentNotification() {
  nextVoiceNotificationPlayTime = 0;

  if (currentNotificationPlaying!=0) {
    wTrig.trackStop(currentNotificationPlaying);
    currentNotificationPlaying = 0;
    currentNotificationPriority = 0;
    return true;
  }
  return false;
}




int AudioHandler::SpaceLeftOnNotificationStack() {
  if (voiceNotificationStackFirst>=VOICE_NOTIFICATION_STACK_SIZE || voiceNotificationStackLast>=VOICE_NOTIFICATION_STACK_SIZE) return 0;
  if (voiceNotificationStackLast>=voiceNotificationStackFirst) return ((VOICE_NOTIFICATION_STACK_SIZE-1) - (voiceNotificationStackLast-voiceNotificationStackFirst));
  return (voiceNotificationStackFirst - voiceNotificationStackLast) - 1;
}


void AudioHandler::PushToNotificationStack(unsigned int notification, unsigned int duration, byte priority) {
  // If the switch stack last index is out of range, then it's an error - return
  if (SpaceLeftOnNotificationStack() == 0) return;

  voiceNotificationNumStack[voiceNotificationStackLast] = notification;
  voiceNotificationDuration[voiceNotificationStackLast] = duration;
  voiceNotificationPriorityStack[voiceNotificationStackLast] = priority;

  voiceNotificationStackLast += 1;
  if (voiceNotificationStackLast == VOICE_NOTIFICATION_STACK_SIZE) {
    // If the end index is off the end, then wrap
    voiceNotificationStackLast = 0;
  }
}



byte AudioHandler::GetTopNotificationPriority() {
  byte startStack = voiceNotificationStackFirst;
  byte endStack = voiceNotificationStackLast;
  if (startStack==endStack) return 0;

  byte topPriorityFound = 0;

  while (startStack!=endStack) {
    if (voiceNotificationPriorityStack[startStack]>topPriorityFound) topPriorityFound = voiceNotificationPriorityStack[startStack];
    startStack += 1;
    if (startStack >= VOICE_NOTIFICATION_STACK_SIZE) startStack = 0;
  }

  return topPriorityFound;
}




boolean AudioHandler::QueueNotification(unsigned short notificationIndex, unsigned short notificationLength, byte priority, unsigned long currentTime) {
#if defined (USE_WAV_TRIGGER) || defined (USE_WAV_TRIGGER_1p3)
  // if everything on the queue has a lower priority, kill all those
  byte topQueuePriority = GetTopNotificationPriority();
  if (priority>topQueuePriority) {
    ClearNotificationStack();  
  }
  if (priority>currentNotificationPriority) {
    StopCurrentNotification();
  }

  // If there's nothing playing, we can play it now
  if (nextVoiceNotificationPlayTime == 0) {
    if (currentBackgroundTrack != BACKGROUND_TRACK_NONE) {
      wTrig.trackFade(currentBackgroundTrack, musicGain - ducking, 500, 0);
    }
    nextVoiceNotificationPlayTime = currentTime + (unsigned long)(notificationLength) * 1000;
    
    wTrig.trackPlayPoly(notificationIndex);
    wTrig.trackGain(notificationIndex, notificationsGain);
    
    currentNotificationPlaying = notificationIndex;
    currentNotificationPriority = priority;
  } else {
    PushToNotificationStack(notificationIndex, notificationLength, priority);
  }
#endif

  return true;
}


void AudioHandler::ServiceNotificationQueue(unsigned long currentTime) {
#if defined (USE_WAV_TRIGGER) || defined (USE_WAV_TRIGGER_1p3)
  if (nextVoiceNotificationPlayTime != 0 && currentTime > nextVoiceNotificationPlayTime) {

    byte nextPriority = 0;
    unsigned int nextNotification = VOICE_NOTIFICATION_STACK_EMPTY;
    unsigned int nextDuration = 0;
    
    // Current notification done, see if there's another
    if (voiceNotificationStackFirst != voiceNotificationStackLast) {
      nextPriority = voiceNotificationPriorityStack[voiceNotificationStackFirst];;
      nextNotification = voiceNotificationNumStack[voiceNotificationStackFirst];
      nextDuration = voiceNotificationDuration[voiceNotificationStackFirst];;

      voiceNotificationStackFirst += 1;
      if (voiceNotificationStackFirst >= VOICE_NOTIFICATION_STACK_SIZE) voiceNotificationStackFirst = 0;    
    }
        
    if (nextNotification != VOICE_NOTIFICATION_STACK_EMPTY) {
      if (currentBackgroundTrack != BACKGROUND_TRACK_NONE) {
        wTrig.trackFade(currentBackgroundTrack, musicGain - ducking, 500, 0);
      }
      nextVoiceNotificationPlayTime = currentTime + (unsigned long)(nextDuration) * 1000;
      wTrig.trackPlayPoly(nextNotification);
      wTrig.trackGain(nextNotification, notificationsGain);
      currentNotificationPlaying = nextNotification;
      currentNotificationPriority = nextPriority;
    } else {
      // No more notifications -- set the volume back up and clear the variable
      if (currentBackgroundTrack != BACKGROUND_TRACK_NONE) {
        wTrig.trackFade(currentBackgroundTrack, musicGain, 1500, 0);
      }
      nextVoiceNotificationPlayTime = 0;
      currentNotificationPlaying = 0;
      currentNotificationPriority = 0;
    }
  }
#endif
}



boolean AudioHandler::StopAllNotifications() {
  ClearNotificationStack();
  return StopCurrentNotification();
}

boolean AudioHandler::StopAllSoundFX() {
#if defined(USE_WAV_TRIGGER) || defined(USE_WAV_TRIGGER_1p3)
  wTrig.stopAllTracks();
#endif
  ClearSoundCardQueue();
  ClearSoundQueue();
  return false;
}


boolean AudioHandler::StopAllAudio() {
  boolean anythingPlaying = false;
  if (StopAllMusic()) anythingPlaying = true;
  if (StopAllNotifications()) anythingPlaying = true;
  if (StopAllSoundFX()) anythingPlaying = true;
  return anythingPlaying;  
}

void AudioHandler::InitSB300Registers() {
#ifdef BALLY_STERN_OS_USE_SB300
  BSOS_PlaySB300SquareWave(1, 0x00); // Write 0x00 to CR2 (Timer 2 off, continuous mode, 16-bit, C2 clock, CR3 set)
  BSOS_PlaySB300SquareWave(0, 0x00); // Write 0x00 to CR3 (Timer 3 off, continuous mode, 16-bit, C3 clock, not prescaled)
  BSOS_PlaySB300SquareWave(1, 0x01); // Write 0x00 to CR2 (Timer 2 off, continuous mode, 16-bit, C2 clock, CR1 set)
  BSOS_PlaySB300SquareWave(0, 0x00); // Write 0x00 to CR1 (Timer 1 off, continuous mode, 16-bit, C1 clock, timers allowed)
#endif
}


void AudioHandler::PlaySB300StartupBeep() {
#ifdef BALLY_STERN_OS_USE_SB300
  BSOS_PlaySB300SquareWave(1, 0x92); // Write 0x92 to CR2 (Timer 2 on, continuous mode, 16-bit, E clock, CR3 set)
  BSOS_PlaySB300SquareWave(0, 0x92); // Write 0x92 to CR3 (Timer 3 on, continuous mode, 16-bit, E clock, not prescaled)
  BSOS_PlaySB300SquareWave(4, 0x02); // Set Timer 2 to 0x0200
  BSOS_PlaySB300SquareWave(5, 0x00); 
  BSOS_PlaySB300SquareWave(6, 0x80); // Set Timer 3 to 0x8000
  BSOS_PlaySB300SquareWave(7, 0x00);
  BSOS_PlaySB300Analog(0, 0x02);
#endif
}


void AudioHandler::ClearSoundCardQueue() {
#ifdef BALLY_STERN_OS_USE_SB300
  for (int count=0; count<SOUND_CARD_QUEUE_SIZE; count++) {
    soundCardQueue[count].playTime = 0;
  }
#endif
}


void AudioHandler::ClearSoundQueue() {
  for (int count=0; count<SOUND_QUEUE_SIZE; count++) {
    soundQueue[count].playTime = 0;
  }
}


boolean AudioHandler::PlaySound(unsigned short soundIndex, byte audioType, byte overrideVolume) {

  boolean soundPlayed = false;
  int gain = soundFXGain;
  if (overrideVolume!=0xFF) gain = ConvertVolumeSettingToGain(overrideVolume);

  if (audioType==AUDIO_PLAY_TYPE_CHIMES) {
#if (BALLY_STERN_OS_HARDWARE_REV==2) && defined(BALLY_STERN_OS_USE_SB100)
    BSOS_PlaySB100Chime((byte)soundIndex);
    soundPlayed = true;
#endif     
  } else if (audioType==AUDIO_PLAY_TYPE_ORIGINAL_SOUNDS) {
#ifdef BALLY_STERN_OS_USE_DASH51
    BSOS_PlaySoundDash51((byte)soundIndex);
    soundPlayed = true;
#endif
#ifdef BALLY_STERN_OS_USE_SQUAWK_AND_TALK
    BSOS_PlaySoundSquawkAndTalk((byte)soundIndex);
    soundPlayed = true;
#endif
#ifdef BALLY_STERN_OS_USE_SB100
    BSOS_PlaySB100((byte)soundIndex);
    soundPlayed = true;
#endif       
  } else if (audioType==AUDIO_PLAY_TYPE_WAV_TRIGGER) {
#if defined(USE_WAV_TRIGGER) || defined(USE_WAV_TRIGGER_1p3)
#ifdef USE_WAV_TRIGGER
    wTrig.trackStop(soundIndex);
#endif

    wTrig.trackPlayPoly(soundIndex);
    wTrig.trackGain(soundIndex, gain);
    soundPlayed = true;
#endif    
  }

  return soundPlayed;  
}


boolean AudioHandler::FadeSound(unsigned short soundIndex, int fadeGain, int numMilliseconds, boolean stopTrack) {
  boolean soundFaded = false;
#if defined(USE_WAV_TRIGGER) || defined(USE_WAV_TRIGGER_1p3)
  wTrig.trackFade(soundIndex, fadeGain, numMilliseconds, stopTrack);
  soundFaded = true;
#endif  
  return soundFaded;
}



boolean AudioHandler::QueueSound(unsigned short soundIndex, byte audioType, unsigned long timeToPlay, byte overrideVolume) {
  for (int count=0; count<SOUND_QUEUE_SIZE; count++) {
    if (soundQueue[count].playTime==0) {
      soundQueue[count].soundIndex = soundIndex;
      soundQueue[count].audioType = audioType;
      soundQueue[count].playTime = timeToPlay;
      soundQueue[count].overrideVolume = overrideVolume;
      return true;
    }
  }
  
  return false;
}


boolean AudioHandler::QueueSoundCardCommand(byte scFunction, byte scRegister, byte scData, unsigned long startTime) {
#ifdef BALLY_STERN_OS_USE_SB300
  for (int count=0; count<SOUND_QUEUE_SIZE; count++) {
    if (soundCardQueue[count].playTime==0) {
      soundCardQueue[count].soundFunction = scFunction;
      soundCardQueue[count].soundRegister = scRegister;
      soundCardQueue[count].soundByte = scData;
      soundCardQueue[count].playTime = startTime;
      return true;
    }
  }
#else 
  // Phony stuff to get rid of warnings
  unsigned long totalval = scFunction + scRegister + scData + startTime;
  totalval += 1;
#endif
  return false;
}


boolean AudioHandler::ServiceSoundQueue(unsigned long currentTime) {
  boolean soundCommandSent = false;
  for (int count=0; count<SOUND_QUEUE_SIZE; count++) {
    if (soundQueue[count].playTime!=0 && soundQueue[count].playTime<currentTime) {
      PlaySound(soundQueue[count].soundIndex, soundQueue[count].audioType, soundQueue[count].overrideVolume);
      soundQueue[count].playTime = 0;
      soundCommandSent = true;
    }
  }

  return soundCommandSent;
}

boolean AudioHandler::ServiceSoundCardQueue(unsigned long currentTime) {
#ifdef BALLY_STERN_OS_USE_SB300
  boolean soundCommandSent = false;
  for (int count=0; count<SOUND_CARD_QUEUE_SIZE; count++) {
    if (soundCardQueue[count].playTime!=0 && soundCardQueue[count].playTime<currentTime) {
      if (soundCardQueue[count].soundFunction==SB300_SOUND_FUNCTION_SQUARE_WAVE) {
        BSOS_PlaySB300SquareWave(soundCardQueue[count].soundRegister, soundCardQueue[count].soundByte);   
      } else if (soundCardQueue[count].soundFunction==SB300_SOUND_FUNCTION_ANALOG) {
        BSOS_PlaySB300Analog(soundCardQueue[count].soundRegister, soundCardQueue[count].soundByte);   
      }
      soundCardQueue[count].playTime = 0;
      soundCommandSent = true;
    }
  }

  return soundCommandSent;
#else 
  // Phony stuff to get rid of warnings
  unsigned long totalval = currentTime;
  totalval += 1;
  return false;
#endif 
}


boolean AudioHandler::PlayBackgroundSoundtrack(AudioSoundtrack *soundtrackArray, unsigned short numSoundtrackEntries, unsigned long currentTime, boolean randomOrder) {
  StopAllMusic();
  if (soundtrackArray==NULL) return false;

  curSoundtrack = soundtrackArray;
  curSoundtrackEntries = numSoundtrackEntries; 
  soundtrackRandomOrder = randomOrder;
  if (currentTime!=0) backgroundSongEndTime = currentTime-1;
  else backgroundSongEndTime = 0;
  
  return true;
}

boolean AudioHandler::PlayBackgroundSong(unsigned short trackIndex, boolean loopTrack) {
  StopAllMusic();
  boolean trackPlayed = false;

  if (trackIndex!=BACKGROUND_TRACK_NONE) {
    currentBackgroundTrack = trackIndex;        
#if defined(USE_WAV_TRIGGER) || defined(USE_WAV_TRIGGER_1p3)
#ifdef USE_WAV_TRIGGER_1p3
    wTrig.trackPlayPoly(trackIndex, true);
    trackPlayed = true;
#else
    wTrig.trackPlayPoly(trackIndex);
    trackPlayed = true;
#endif
    if (loopTrack) wTrig.trackLoop(trackIndex, true);
    wTrig.trackGain(trackIndex, musicGain);
#endif
  }

  return trackPlayed;
  
}


void AudioHandler::StartNextSoundtrackSong(unsigned long currentTime) {

  unsigned int retSong = (currentTime%curSoundtrackEntries);
  boolean songRecentlyPlayed = false;

  unsigned int songCount = 0;
  for (songCount=0; songCount<curSoundtrackEntries; songCount++) {
    for (byte count=0; count<NUMBER_OF_SONGS_REMEMBERED; count++) {
      if (lastSongsPlayed[count]==curSoundtrack[retSong].TrackIndex) {
        songRecentlyPlayed = true;        
        break;
      }
    }
    if (!songRecentlyPlayed) break;
    retSong = (retSong+1);
    songRecentlyPlayed = false;
    if (retSong>=curSoundtrackEntries) retSong = 0;
  }

  // Record this song in the array
  for (byte count=(NUMBER_OF_SONGS_REMEMBERED-1); count>0; count--) lastSongsPlayed[count] = lastSongsPlayed[count-1];
  lastSongsPlayed[0] = curSoundtrack[retSong].TrackIndex;

  backgroundSongEndTime = (((unsigned long)curSoundtrack[retSong].TrackLength) * 1000) + currentTime;
  
  if (currentBackgroundTrack!=BACKGROUND_TRACK_NONE) {
#if defined(USE_WAV_TRIGGER) || defined(USE_WAV_TRIGGER_1p3)
    wTrig.trackFade(currentBackgroundTrack, -80, 2000, 1);
#endif
  }
  currentBackgroundTrack = curSoundtrack[retSong].TrackIndex;

#if defined(USE_WAV_TRIGGER) || defined(USE_WAV_TRIGGER_1p3)
#ifdef USE_WAV_TRIGGER_1p3
  wTrig.trackPlayPoly(currentBackgroundTrack, true);
#else
  wTrig.trackPlayPoly(currentBackgroundTrack);
#endif
  wTrig.trackGain(currentBackgroundTrack, musicGain);
#endif

}




void AudioHandler::ManageBackgroundSong(unsigned long currentTime) {
  if (curSoundtrack==NULL) return; 

  if (currentTime>=backgroundSongEndTime) {
    StartNextSoundtrackSong(currentTime);
  }
}


boolean AudioHandler::Update(unsigned long currentTime) {
  ManageBackgroundSong(currentTime);
  ServiceSoundQueue(currentTime);
  ServiceSoundCardQueue(currentTime);
  ServiceNotificationQueue(currentTime);
  return true;
}
