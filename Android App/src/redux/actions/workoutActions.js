export const gpsUpdate = (speed, distance, altitude) => ({
  type: 'gpsUpdate',
  speed: speed,
  distance: distance,
  alt: altitude,
});
export const workoutStarted = () => ({
  type: 'workoutStarted',
});

export const workoutEnded = () => ({
  type: 'workoutEnded',
});

export const incDuration = () => ({
  type: 'incDuration',
});
export const reset = () => ({
  type: 'resetWorkout',
});
export const calcGain = () => ({
  type: 'calcGain',
});
