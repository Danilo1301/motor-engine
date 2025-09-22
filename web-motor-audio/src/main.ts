import './style.css';

class MotorAudio {
  private audioCtx?: AudioContext;
  private source?: AudioBufferSourceNode;
  private gainNode?: GainNode;
  private buffer?: AudioBuffer;

  public rpm: number = 1000;
  public marcha: number = 1;

  async init() {
    this.audioCtx = new AudioContext();
    this.gainNode = this.audioCtx.createGain();
    this.gainNode.connect(this.audioCtx.destination);

    const res = await fetch('engine.wav');
    const arrayBuffer = await res.arrayBuffer();
    this.buffer = await this.audioCtx.decodeAudioData(arrayBuffer);
  }

  play() {
    if (!this.buffer || !this.audioCtx || !this.gainNode) return;

    this.source = this.audioCtx.createBufferSource();
    this.source.buffer = this.buffer;
    this.source.loop = true;

    this.source.playbackRate.value = this.rpmToPitch(this.rpm);

    this.source.connect(this.gainNode);
    this.source.start();
  }

  stop() {
    this.source?.stop();
  }

  setRPM(rpm: number) {
    this.rpm = Math.max(800, Math.min(rpm, 8000));
    if (this.source) {
      this.source.playbackRate.value = this.rpmToPitch(this.rpm);
    }
  }

  changeGear(newGear: number) {
    this.marcha = Math.max(1, newGear);
    this.setRPM(this.rpm * 0.5);
  }

  private rpmToPitch(rpm: number): number {
    const idleRPM = 800;
    const maxRPM = 8000;

    // normaliza rpm dentro da faixa 0..1
    const t = (rpm - idleRPM) / (maxRPM - idleRPM);
    const clamped = Math.max(0, Math.min(1, t));

    // base da primeira marcha
    const minPitch = 0.8;
    const maxPitchBase = 1.7;

    // cada marcha aumenta o pitch máximo em +0.2
    const maxPitch = maxPitchBase + (this.marcha - 1) * 0.2;

    return minPitch + (maxPitch - minPitch) * clamped;
  }
}

// -------------------
// Main
// -------------------
const motor = new MotorAudio();
const keys: Record<string, boolean> = {};
let started = false;

function setupInput() {
  window.addEventListener('keydown', (e) => {
    keys[e.code] = true;
    if (e.code === 'ShiftLeft') {
      motor.changeGear(motor.marcha + 1);
    }
  });

  window.addEventListener('keyup', (e) => {
    keys[e.code] = false;
  });
}

async function startSimulation() {
  if (started) return;
  started = true;

  await motor.init();
  motor.play();
  setupInput();

  requestAnimationFrame(loop);
}

function loop() {
  if (keys['KeyW']) {
    motor.setRPM(motor.rpm + 30);
  }
  if (keys['KeyS']) {
    motor.setRPM(motor.rpm - 50);
  }

  document.getElementById('status')!.textContent =
    `Marcha: ${motor.marcha} | RPM: ${motor.rpm.toFixed(0)}`;

  requestAnimationFrame(loop);
}

document.getElementById('startBtn')!.addEventListener('click', startSimulation);