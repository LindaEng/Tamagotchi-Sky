// -------- Animation Data --------

struct Animation {
  int* frames;
  int frameCount;
};

int happyFrames[]  = {0,1,2};
int normalFrames[] = {0,1,2};
int tiredFrames[]  = {0,1,2};
int deadFrames[]   = {0};

Animation happyAnim  = { happyFrames, 3 };
Animation normalAnim = { normalFrames, 3 };
Animation tiredAnim  = { tiredFrames, 3 };
Animation deadAnim   = { deadFrames, 1 };


// -------- Pet Mood --------

enum PetMood {
  HAPPY,
  NORMAL,
  TIRED,
  DEAD
};


// -------- Pet Structure --------

struct Pet {
  int exercise = 100;
  int coding = 100;
  int music = 100;

  bool alive = true;
  PetMood mood = NORMAL;
};

Pet pet;

bool waitingForRevive = false;
// -------- Timers --------

unsigned long lastDecay = 0;
const int decayInterval = 5000;

int currentFrame = 0;
unsigned long lastFrameTime = 0;
const int frameInterval = 400;

PetMood lastMood = NORMAL;


// -------- Mood Logic --------

void updateMood() {

  int avg = (pet.exercise + pet.coding + pet.music) / 3;

  if (!pet.alive)
    pet.mood = DEAD;

  else if (avg > 80)
    pet.mood = HAPPY;

  else if (avg > 50)
    pet.mood = NORMAL;

  else
    pet.mood = TIRED;
}


// -------- Animation Logic --------

void updateAnimation() {

  if (millis() - lastFrameTime < frameInterval)
    return;

  Animation currentAnim;

  switch (pet.mood) {

    case HAPPY:
      currentAnim = happyAnim;
      break;

    case NORMAL:
      currentAnim = normalAnim;
      break;

    case TIRED:
      currentAnim = tiredAnim;
      break;

    case DEAD:
      currentAnim = deadAnim;
      break;
  }

  if (pet.mood != lastMood) {
    currentFrame = 0;
    lastMood = pet.mood;
  }

  currentFrame++;

  if (currentFrame >= currentAnim.frameCount)
    currentFrame = 0;

  Serial.print("Mood: ");

  switch (pet.mood) {
    case HAPPY: Serial.print("HAPPY "); break;
    case NORMAL: Serial.print("NORMAL "); break;
    case TIRED: Serial.print("TIRED "); break;
    case DEAD: Serial.print("DEAD "); break;
  }

  Serial.print("Frame: ");
  Serial.println(currentAnim.frames[currentFrame]);

  lastFrameTime = millis();
}


// -------- Stat Decay --------

void decayStats() {

  pet.exercise -= random(0,10);
  pet.coding   -= random(0,10);
  pet.music    -= random(0,10);

  if (pet.exercise <= 0 || pet.coding <= 0 || pet.music <= 0)
    pet.alive = false;
}


// -------- Actions --------

void doExercise() {

  pet.exercise += 10;

  Serial.print("Exercise increased! ");
  Serial.println(pet.exercise);
}

void doCoding() {

  pet.coding += 10;

  Serial.print("Coding increased! ");
  Serial.println(pet.coding);
}

void doMusic() {

  pet.music += 10;

  Serial.print("Music increased! ");
  Serial.println(pet.music);
}

void doRestart() {
  pet.music = 100;
  pet.coding = 100;
  pet.exercise = 100;
  pet.alive = true;
  Serial.println("We brought your pet back to life~ ");
}


// -------- Setup --------

void setup() {

  Serial.begin(9600);
  randomSeed(analogRead(0));

  Serial.println("Tamagotchi started.");
}


// -------- Main Loop --------

void loop() {

  // input
  if (Serial.available()) {
    char command = Serial.read();
    if (command == 'e') doExercise();
    if (command == 'c') doCoding();
    if (command == 'm') doMusic();
  }

  // death
  if (!pet.alive) {

    if (!waitingForRevive) {
      Serial.println("Your pet is ded ;_;");
      Serial.println("Press y to revive or n to abandon.");
      waitingForRevive = true;
    }

    if (Serial.available()) {
      char command = Serial.read();

      if (command == 'y') {
        doRestart();
        waitingForRevive = false;
      }

      if (command == 'n') {
        Serial.println("Game over.");
        while(true);
      }
    }

    return;
  }

  // animation
  updateAnimation();

  // stat decay
  if (millis() - lastDecay > decayInterval) {

    decayStats();
    updateMood();

    Serial.print("Exercise: ");
    Serial.println(pet.exercise);

    Serial.print("Coding: ");
    Serial.println(pet.coding);

    Serial.print("Music: ");
    Serial.println(pet.music);

    Serial.println("-----");

    lastDecay = millis();
  }
}