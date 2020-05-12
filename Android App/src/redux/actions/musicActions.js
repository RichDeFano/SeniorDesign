// Login
export const songUpdate = (
  trackName,
  artistName,
  trackLength,
  isPaused,
  position,
) => ({
  type: 'SongUpdate',
  track: trackName,
  artist: artistName,
  track_length: trackLength,
  isPlaying: !isPaused,
  playbackPosition: position,
});
